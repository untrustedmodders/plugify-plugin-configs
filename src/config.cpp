#include "config.hpp"
#include <format>
#include <string>

namespace pcf
{
	template<typename VariantType, typename T, std::size_t index = 0>
	constexpr std::size_t variant_index() {
		static_assert(std::variant_size_v<VariantType> > index, "Type not found in variant");
		if constexpr (index == std::variant_size_v<VariantType>) {
			return index;
		}
		else if constexpr (std::is_same_v<std::variant_alternative_t<index, VariantType>, T>) {
			return index;
		}
		else {
			return variant_index<VariantType, T, index + 1>();
		}
	}

	Config::Detail::Node::Ptr Config::Detail::Node::NewNode()
	{
		return Ptr{ new Config::Detail::Node() };
	}

	Config::Detail::Node::Ptr Config::Detail::Node::NewNullNode()
	{
		return Ptr{ new Config::Detail::Node(nullptr) };
	}

	Config::Detail::Node::Node() = default;

	Config::Detail::Node::Node(nullptr_t)
		: _storage(nullptr), _objCache{}
	{
	}

	Config::NodeType Config::Detail::Node::GetType() const
	{
		static_assert(static_cast<size_t>(NodeType::Null) == 0 && variant_index<StorageType, NullType>() == 0);
		static_assert(static_cast<size_t>(NodeType::Bool) == 1 && variant_index<StorageType, BoolType>() == 1);
		static_assert(static_cast<size_t>(NodeType::Int) == 2 && variant_index<StorageType, NumberType>() == 2);
		static_assert(static_cast<size_t>(NodeType::Float) == 3 && variant_index<StorageType, FloatType>() == 3);
		static_assert(static_cast<size_t>(NodeType::String) == 4 && variant_index<StorageType, StringType>() == 4);
		static_assert(static_cast<size_t>(NodeType::Object) == 5 && variant_index<StorageType, ObjectType>() == 5);
		static_assert(static_cast<size_t>(NodeType::Array) == 6 && variant_index<StorageType, ArrayType>() == 6);

		if (_storage.index() == std::variant_npos) {
			return NodeType::Null;
		}
		return static_cast<NodeType>(_storage.index());
	}

	void Config::Detail::Node::SetObject()
	{
		_storage = ObjectType{};
		_objCache = std::get<ObjectType>(_storage).end();
	}

	void Config::Detail::Node::SetArray()
	{
		_storage = ArrayType{};
		_arrCache = std::get<ArrayType>(_storage).end();
	}

	Config::Detail::Node* Config::Detail::Node::PushBack()
	{
		if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			_arrCache = arr->end();
			return arr->emplace_back(NewNode()).get();
		}

		return nullptr;
	}

	Config::Detail::Node* Config::Detail::Node::PushBackNull()
	{
		if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			_arrCache = arr->end();
			return arr->emplace_back(NewNullNode()).get();
		}

		return nullptr;
	}

	bool Config::Detail::Node::Get(bool defaultValue) const
	{
		static_assert(std::is_same_v<BoolType, bool>);
		if (const bool* const value = std::get_if<bool>(&_storage)) {
			return *value;
		}
		return defaultValue;
	}

	int64_t Config::Detail::Node::Get(int64_t defaultValue) const
	{
		static_assert(std::is_same_v<NumberType, int64_t>);
		if (const int64_t* const value = std::get_if<NumberType>(&_storage)) {
			return *value;
		}
		return defaultValue;
	}

	double Config::Detail::Node::Get(double defaultValue) const
	{
		static_assert(std::is_same_v<FloatType, double>);
		if (const double* const value = std::get_if<double>(&_storage)) {
			return *value;
		}
		return defaultValue;
	}

	plg::string Config::Detail::Node::Get(std::string_view defaultValue) const
	{
		static_assert(std::is_same_v<StringType, plg::string>);
		if (const plg::string* const value = std::get_if<plg::string>(&_storage)) {
			return *value;
		}
		return { defaultValue };
	}

	Config::Detail::Node* Config::Detail::Node::At(std::string_view key)
	{
		if (auto* const object = std::get_if<ObjectType>(&_storage)) {
			if (const auto it = object->find(key); it != object->end()) {
				_objCache = it;
				return it->second.get();
			}
		}

		return nullptr;
	}

	Config::Detail::Node* Config::Detail::Node::At(int n)
	{
		if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			if (n >= 0) {
				if (n < arr->size()) {
					const auto it = arr->begin() + n;
					_arrCache = it;
					return it->get();
				}
			}
			else {
				const auto i = static_cast<size_t>(-n) - 1;
				if (i < arr->size()) {
					const auto it = arr->rbegin() + i;
					_arrCache = it.base();
					return it->get();
				}
			}
		}

		return nullptr;
	}

	Config::Detail::Node* Config::Detail::Node::First()
	{
		if (auto* const object = std::get_if<ObjectType>(&_storage)) {
			if (const auto it = object->begin(); it != object->end()) {
				_objCache = it;
				return it->second.get();
			}
		}
		else if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			if (const auto it = arr->begin(); it != arr->end()) {
				_arrCache = it;
				return it->get();
			}
		}

		return nullptr;
	}

	Config::Detail::Node* Config::Detail::Node::Last()
	{
		if (auto* const object = std::get_if<ObjectType>(&_storage)) {
			if (const auto it = object->rbegin(); it != object->rend()) {
				_objCache = it.base();
				return it->second.get();
			}
		}
		else if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			if (const auto it = arr->rbegin(); it != arr->rend()) {
				_arrCache = it.base();
				return it->get();
			}
		}

		return nullptr;
	}

	Config::Detail::Node* Config::Detail::Node::Next(Node* node)
	{
		if (auto* const object = std::get_if<ObjectType>(&_storage)) {
			if (_objCache != object->end() && _objCache->second.get() == node) {
				ObjectType::iterator nextit = _objCache;
				if (++nextit == object->end()) {
					return nullptr;
				}
				_objCache = nextit;
				return nextit->second.get();
			}

			for (auto it = object->begin(); it != object->end(); ++it) {
				if (it->second.get() == node) {
					_objCache = it;
					ObjectType::iterator nextit = it;
					if (++nextit == object->end()) {
						return nullptr;
					}
					_objCache = nextit;
					return nextit->second.get();
				}
			}

			return nullptr;
		}

		if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			if (_arrCache != arr->end() && _arrCache->get() == node) {
				ArrayType::iterator nextit = _arrCache;
				if (++nextit == arr->end()) {
					return nullptr;
				}
				_arrCache = nextit;
				return nextit->get();
			}

			for (auto it = arr->begin(); it != arr->end(); ++it) {
				if (it->get() == node) {
					_arrCache = it;
					ArrayType::iterator nextit = it;
					if (++nextit == arr->end()) {
						return nullptr;
					}
					_arrCache = nextit;
					return nextit->get();
				}
			}

			return nullptr;
		}

		return nullptr;
	}
	
	Config::Detail::Node* Config::Detail::Node::Prev(Node* node)
	{
		if (auto* const object = std::get_if<ObjectType>(&_storage)) {
			if (_objCache != object->end() && _objCache->second.get() == node) {
				auto nextit = std::reverse_iterator<ObjectType::iterator>(_objCache);
				if (++nextit == object->rend()) {
					return nullptr;
				}
				_objCache = nextit.base();
				return nextit->second.get();
			}

			for (auto it = object->rbegin(); it != object->rend(); ++it) {
				if (it->second.get() == node) {
					_objCache = it.base();
					std::reverse_iterator<ObjectType::iterator> nextit = it;
					if (++nextit == object->rend()) {
						return nullptr;
					}
					_objCache = nextit.base();
					return nextit->second.get();
				}
			}

			return nullptr;
		}

		if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			if (_arrCache != arr->end() && _arrCache->get() == node) {
				auto nextit = std::reverse_iterator<ArrayType::iterator>(_arrCache);
				if (++nextit == arr->rend()) {
					return nullptr;
				}
				_arrCache = nextit.base();
				return nextit->get();
			}

			for (auto it = arr->rbegin(); it != arr->rend(); ++it) {
				if (it->get() == node) {
					_arrCache = it.base();
					std::reverse_iterator<ArrayType::iterator> nextit = it;
					if (++nextit == arr->rend()) {
						return nullptr;
					}
					_arrCache = nextit.base();
					return nextit->get();
				}
			}

			return nullptr;
		}

		return nullptr;
	}

	Config::Detail::Node* Config::Detail::Node::Create(std::string_view key)
	{
		if (auto* const object = std::get_if<ObjectType>(&_storage)) {
			const auto it = object->emplace(key, NewNode());
			return it->second.get();
		}

		return nullptr;
	}

	std::pair<Config::Detail::Node::Ptr, Config::Detail::Node*> Config::Detail::Node::Extract(Node* node)
	{
		std::pair<Ptr, Node*> result{};
		auto& [extractedNode, nextNode] = result;

		if (auto* const object = std::get_if<ObjectType>(&_storage)) {
			ObjectType::iterator nodeit = object->end();
			if (_objCache != object->end() && _objCache->second.get() == node) {
				nodeit = _objCache;
			}
			else {
				for (auto it = object->begin(); it != object->end(); ++it) {
					if (it->second.get() == node) {
						nodeit = it;
						break;
					}
				}
			}

			if (nodeit != object->end()) {
				_objCache = object->end();
				ObjectType::iterator nextit = std::next(nodeit);
				if (nextit != object->end()) {
					_objCache = nextit;
					nextNode = nextit->second.get();
				}
				extractedNode = std::move(nodeit->second);
				object->erase(nodeit);
			}
		}
		else if (auto* const arr = std::get_if<ArrayType>(&_storage)) {
			ArrayType::iterator nodeit = arr->end();

			if (_arrCache != arr->end() && _arrCache->get() == node) {
				nodeit = _arrCache;
			}
			else {
				for (auto it = arr->begin(); it != arr->end(); ++it) {
					if (it->get() == node) {
						nodeit = it;
						break;
					}
				}
			}

			if (nodeit != arr->end()) {
				extractedNode = std::move(*nodeit);
				ArrayType::iterator nextit = arr->erase(nodeit);
				if (nextit != arr->end()) {
					nextNode = nextit->get();
				}
				_arrCache = nextit;
			}
		}

		return result;
	}

	plg::string Config::Detail::Node::ToJsonString() const
	{
		using namespace plg::string_literals;

		plg::string json;

		if (std::get_if<NullType>(&_storage)) {
			json = "null"s;
		}
		else if (const bool* const boolValue = std::get_if<BoolType>(&_storage)) {
			json = *boolValue ? "true"s : "false"s;
		}
		else if (const int64_t* const numberValue = std::get_if<NumberType>(&_storage)) {
			json = plg::to_string(*numberValue);
		}
		else if (const double* const floatValue = std::get_if<FloatType>(&_storage)) {
			json = plg::to_string(*floatValue);
		}
		else if (const plg::string* const stringValue = std::get_if<StringType>(&_storage)) {
			std::format_to(std::back_inserter(json), "\"{}\"", *stringValue);
		}
		else if (const ObjectType* const objValue = std::get_if<ObjectType>(&_storage)) {
			json.push_back('{');
			if (!objValue->empty()) {
				for (const auto& [name, node] : *objValue) {
					std::format_to(std::back_inserter(json), "\"{}\":{},", name, node->ToJsonString());
				}
				json.erase(json.end() - 1, json.end());
			}
			json.push_back('}');
		}
		else if (const ArrayType* const arrValue = std::get_if<ArrayType>(&_storage)) {
			json.push_back('[');
			if (!arrValue->empty()) {
				for (const auto& item : *arrValue) {
					std::format_to(std::back_inserter(json), "{},", item->ToJsonString());
				}
				json.erase(json.end() - 1, json.end());
			}
			json.push_back(']');
		}

		return json;
	}

	std::unique_ptr<Config> Config::Detail::MakeConfig()
	{
		return std::unique_ptr<Config>(new Config(std::unique_ptr<Detail>(new Detail())));
	}

	Config::Detail::Detail() = default;

	Config::NodeType Config::Detail::GetType() const
	{
		return GetCurrent().GetType();
	}

	bool Config::Detail::IsNull() const
	{
		return GetType() == NodeType::Null;
	}

	bool Config::Detail::IsBool() const
	{
		return GetType() == NodeType::Bool;
	}

	bool Config::Detail::IsInt() const
	{
		return GetType() == NodeType::Int;
	}

	bool Config::Detail::IsFloat() const
	{
		return GetType() == NodeType::Float;
	}

	bool Config::Detail::IsString() const
	{
		return GetType() == NodeType::String;
	}

	bool Config::Detail::IsObject() const
	{
		return GetType() == NodeType::Object;
	}

	bool Config::Detail::IsArray() const
	{
		return GetType() == NodeType::Array;
	}

	void Config::Detail::SetObject()
	{
		GetCurrent().SetObject();
	}

	void Config::Detail::SetArray()
	{
		GetCurrent().SetArray();
	}

	void Config::Detail::PushNull()
	{
		GetCurrent().PushBackNull();
	}

	void Config::Detail::PushBool(bool value)
	{
		Node* const node = GetCurrent().PushBackNull();
		if (node) {
			node->Set(value);
		}
	}

	void Config::Detail::PushInt(int64_t value)
	{
		Node* const node = GetCurrent().PushBackNull();
		if (node) {
			node->Set(value);
		}
	}

	void Config::Detail::PushFloat(double value)
	{
		Node* const node = GetCurrent().PushBackNull();
		if (node) {
			node->Set(value);
		}
	}

	void Config::Detail::PushString(plg::string value)
	{
		Node* const node = GetCurrent().PushBackNull();
		if (node) {
			node->Set(std::move(value));
		}
	}

	void Config::Detail::PushObject()
	{
		GetCurrent().PushBack();
	}

	void Config::Detail::PushArray()
	{
		Node* const node = GetCurrent().PushBackNull();
		if (node) {
			node->SetArray();
		}
	}

	template<typename T>
	T Config::Detail::Get(T defaultValue) const
	{
		return GetCurrent().Get(defaultValue);
	}

	plg::string Config::Detail::Get(std::string_view defaultValue) const
	{
		return GetCurrent().Get(defaultValue);
	}

	bool Config::Detail::JumpFirst()
	{
		Node* const node = GetCurrent().First();
		if (node) {
			_track.push_back(node);
			return true;
		}
		return false;
	}

	bool Config::Detail::JumpLast()
	{
		Node* const node = GetCurrent().Last();
		if (node) {
			_track.push_back(node);
			return true;
		}
		return false;
	}

	bool Config::Detail::JumpNext()
	{
		Node* const parent = GetCurrentParent();
		if (!parent) {
			return false;
		}

		Node* const next = parent->Next(_track.back());
		if (!next) {
			return false;
		}

		_track.pop_back();
		_track.push_back(next);

		return true;
	}
	
	bool Config::Detail::JumpPrev()
	{
		Node* const parent = GetCurrentParent();
		if (!parent) {
			return false;
		}

		Node* const prev = parent->Prev(_track.back());
		if (!prev) {
			return false;
		}

		_track.pop_back();
		_track.push_back(prev);

		return true;
	}

	bool Config::Detail::JumpKey(std::string_view key, bool create)
	{
		auto& top = GetCurrent();
		auto* node = top.At(key);
		if (!node && create) {
			node = top.Create(key);
		}
		if (node) {
			_track.push_back(node);
			return true;
		}
		return false;
	}

	bool Config::Detail::JumpN(int n)
	{
		auto& top = GetCurrent();
		auto* node = top.At(n);
		if (node) {
			_track.push_back(node);
			return true;
		}
		return false;
	}

	bool Config::Detail::JumpBack()
	{
		if (_track.empty()) {
			return false;
		}
		_track.pop_back();
		return true;
	}

	void Config::Detail::JumpRoot()
	{
		_track = {};
	}

	int Config::Detail::Remove()
	{
		Node* const parent = GetCurrentParent();
		if (!parent) {
			_root = Node::NewNode();
			return 0;
		}

		auto&& [nodePtr, next] = parent->Extract(_track.back());
		if (nodePtr) {
			_track.pop_back();
			if (next) {
				_track.push_back(next);
				return 1;
			}
			return -1;
		}

		return 0;
	}
	
	bool Config::Detail::RemoveKey(std::string_view key)
	{
		if (key.empty()) {
			return false;
		}

		auto& top = GetCurrent();
		Node* const node = top.At(key);
		if (node) {
			auto&& [nodePtr, next] = top.Extract(node);
			return static_cast<bool>(nodePtr);
		}
		
		return false;
	}
	
	void Config::Detail::Clear()
	{
		_track.clear();
		_root = Node::NewNode();
	}

	plg::string Config::Detail::NodeToJsonString() const
	{
		return GetCurrent().ToJsonString();
	}

	plg::string Config::Detail::RootToJsonString() const
	{
		return _root->ToJsonString();
	}

	Config::Detail::Node& Config::Detail::GetCurrent() const
	{
		if (!_track.empty()) {
			return *(_track.back());
		}

		return *_root;
	}

	Config::Detail::Node* Config::Detail::GetCurrentParent() const
	{
		auto it = _track.crbegin();
		if (it == _track.crend()) {
			return nullptr;
		}
		if (++it == _track.crend()) {
			return _root.get();
		}
		return *it;
	}
	

	Config::NodeType Config::GetType() const
	{
		return _detail->GetType();
	}

	bool Config::IsNull() const
	{
		return _detail->IsNull();
	}

	bool Config::IsBool() const
	{
		return _detail->IsBool();
	}

	bool Config::IsInt() const
	{
		return _detail->IsInt();
	}

	bool Config::IsFloat() const
	{
		return _detail->IsFloat();
	}

	bool Config::IsString() const
	{
		return _detail->IsString();
	}

	bool Config::IsObject() const
	{
		return _detail->IsObject();
	}

	bool Config::IsArray() const
	{
		return _detail->IsArray();
	}

	void Config::SetNull()
	{
		_detail->Set(nullptr);
	}

	void Config::Set(bool value)
	{
		_detail->Set(value);
	}

	void Config::Set(int32_t value)
	{
		_detail->Set(static_cast<int64_t>(value));
	}

	void Config::Set(int64_t value)
	{
		_detail->Set(value);
	}

	void Config::Set(float value)
	{
		_detail->Set(static_cast<double>(value));
	}

	void Config::Set(double value)
	{
		_detail->Set(value);
	}

	void Config::Set(plg::string value)
	{
		_detail->Set(std::move(value));
	}

	void Config::SetObject()
	{
		_detail->SetObject();
	}

	void Config::SetArray()
	{
		_detail->SetArray();
	}

	void Config::PushNull()
	{
		_detail->PushNull();
	}

	void Config::PushBool(bool value)
	{
		_detail->PushBool(value);
	}

	void Config::PushInt32(int32_t value)
	{
		_detail->PushInt(static_cast<int64_t>(value));
	}

	void Config::PushInt64(int64_t value)
	{
		_detail->PushInt(value);
	}

	void Config::PushFloat(float value)
	{
		_detail->PushFloat(static_cast<double>(value));
	}

	void Config::PushDouble(double value)
	{
		_detail->PushFloat(value);
	}

	void Config::PushString(plg::string value)
	{
		_detail->PushString(std::move(value));
	}

	void Config::PushObject()
	{
		_detail->PushObject();
	}

	void Config::PushArray()
	{
		_detail->PushArray();
	}

	bool Config::GetBool(bool defaultValue) const
	{
		return _detail->Get(defaultValue);
	}

	int32_t Config::GetInt32(int32_t defaultValue) const
	{
		return static_cast<int32_t>(_detail->Get(static_cast<int64_t>(defaultValue)));
	}

	int64_t Config::GetInt64(int64_t defaultValue) const
	{
		return _detail->Get(defaultValue);
	}

	float Config::GetFloat(float defaultValue) const
	{
		return static_cast<float>(_detail->Get(static_cast<double>(defaultValue)));
	}

	double Config::GetDouble(double defaultValue) const
	{
		return _detail->Get(defaultValue);
	}

	plg::string Config::GetString(std::string_view defaultValue) const
	{
		return _detail->Get(defaultValue);
	}

	bool Config::JumpFirst()
	{
		return _detail->JumpFirst();
	}

	bool Config::JumpLast()
	{
		return _detail->JumpLast();
	}

	bool Config::JumpNext()
	{
		return _detail->JumpNext();
	}

	bool Config::JumpPrev()
	{
		return _detail->JumpPrev();
	}

	bool Config::JumpKey(std::string_view key, bool create)
	{
		return _detail->JumpKey(key, create);
	}

	bool Config::JumpN(int n)
	{
		return _detail->JumpN(n);
	}

	bool Config::JumpBack()
	{
		return _detail->JumpBack();
	}

	void Config::JumpRoot()
	{
		_detail->JumpRoot();
	}

	int Config::Remove()
	{
		return _detail->Remove();
	}
	
	bool Config::RemoveKey(std::string_view key)
	{
		return _detail->RemoveKey(key);
	}
	
	void Config::Clear()
	{
		_detail->Clear();
	}

	plg::string Config::NodeToJsonString() const
	{
		return _detail->NodeToJsonString();
	}

	plg::string Config::RootToJsonString() const
	{
		return _detail->RootToJsonString();
	}

	Config::Config(std::unique_ptr<Config::Detail> detail)
		: _detail(std::move(detail))
	{
	}

	Config::~Config() = default;
}
