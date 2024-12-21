#pragma once
#include <plugify-configs/plugify-configs.hpp>
#include <plugify/vector.hpp>
#include <plugify/string.hpp>
#include <map>
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
			using ObjectType = std::map<plg::string, Ptr>;
			using ArrayType = plg::vector<Ptr>;
			using StorageType = std::variant<NullType, BoolType, NumberType, FloatType, StringType, ObjectType, ArrayType>;
			template<class T>
			static constexpr bool is_storable_v = std::is_same_v<T, NullType> || std::is_same_v<T, BoolType> || std::is_same_v<T, NumberType> 
				|| std::is_same_v<T, FloatType> || std::is_same_v<T, StringType> || std::is_same_v<T, ObjectType> || std::is_same_v<T, ArrayType>;

		public:
			Node();

		private:
			StorageType _storage = ObjectType{};
		};

	public:
		static std::unique_ptr<Config> MakeConfig();

	private:
		Detail();

	private:
		Node::Ptr _root = std::make_unique<Config::Detail::Node>();
	};
}
