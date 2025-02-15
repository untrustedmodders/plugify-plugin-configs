#pragma once
#include <plugify-configs/methods.hpp>
#include <plugify-configs/plugify-configs.hpp>
#include <plugify/vector.hpp>
#include <plugify/string.hpp>
#include <map>
#include <stack>
#include <variant>
#include <memory>

namespace pcf
{
	class Config::Detail
	{
	private:
		class Node
		{
		public:
			using Ptr = std::unique_ptr<Node>;
			using NullType = std::nullptr_t;
			using BoolType = bool;
			using NumberType = int64_t;
			using FloatType = double;
			using StringType = plg::string;
			using ObjectType = std::multimap<plg::string, Ptr>;
			using ArrayType = plg::vector<Ptr>;
			using StorageType = std::variant<NullType, BoolType, NumberType, FloatType, StringType, ObjectType, ArrayType>;
			template<class T>
			static constexpr bool is_storable_v = std::is_same_v<T, NullType> || std::is_same_v<T, BoolType> || std::is_same_v<T, NumberType> 
				|| std::is_same_v<T, FloatType> || std::is_same_v<T, StringType> || std::is_same_v<T, ObjectType> || std::is_same_v<T, ArrayType>;

		public:
			Node();

		public:
			NodeType GetType() const;

			template<class T>
			void Set(T value);

		private:
			StorageType _storage = ObjectType{};
		};

	public:
		static std::unique_ptr<Config> MakeConfig();

		NodeType GetType() const;
		bool IsNull() const;
		bool IsBool() const;
		bool IsInt() const;
		bool IsFloat() const;
		bool IsString() const;
		bool IsObject() const;
		bool IsArray() const;

		template<typename T> requires Config::Detail::Node::is_storable_v<T>
		void Set(T value);

	private:
		Detail();

		Node& GetCurrent() const;

	private:
		Node::Ptr _root = std::make_unique<Config::Detail::Node>();
		std::stack<Node*> _track;
	};

	template<class T>
	void Config::Detail::Node::Set(T value)
	{
		static_assert(std::is_scalar_v<T>);
		_storage = value;
	}

	template <typename T> requires Config::Detail::Node::is_storable_v<T>
	void Config::Detail::Set(T value)
	{
		_root->Set(value);
	}
}
