#include "config.hpp"

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

	Config::Detail::Node::Node() = default;

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

	Config::Detail::Node& Config::Detail::GetCurrent() const
	{
		if (!_track.empty()) {
			return *(_track.top());
		}

		return *_root;
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

	void Config::Set(nullptr_t)
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

	Config::Config(std::unique_ptr<Config::Detail> detail)
		: _detail(std::move(detail))
	{
	}

	Config::~Config() = default;
}
