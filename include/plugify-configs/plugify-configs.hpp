#pragma once
#include "methods.hpp"
#include <plugify_configs_export.h>
#include <memory>

namespace pcf
{
	class Config
	{
	public:
		enum class NodeType
		{
			Null,
			Bool,
			Int,
			Float,
			String,
			Object,
			Array,
		};

	public:
		PLUGIFY_CONFIGS_API virtual ~Config();

		PLUGIFY_CONFIGS_API NodeType GetType() const;
		PLUGIFY_CONFIGS_API bool IsNull() const;
		PLUGIFY_CONFIGS_API bool IsBool() const;
		PLUGIFY_CONFIGS_API bool IsInt() const;
		PLUGIFY_CONFIGS_API bool IsFloat() const;
		PLUGIFY_CONFIGS_API bool IsString() const;
		PLUGIFY_CONFIGS_API bool IsObject() const;
		PLUGIFY_CONFIGS_API bool IsArray() const;

		PLUGIFY_CONFIGS_API void Set(nullptr_t);
		PLUGIFY_CONFIGS_API void Set(bool value);
		PLUGIFY_CONFIGS_API void Set(int32_t value);
		PLUGIFY_CONFIGS_API void Set(int64_t value);
		PLUGIFY_CONFIGS_API void Set(float value);
		PLUGIFY_CONFIGS_API void Set(double value);

		PLUGIFY_CONFIGS_API plg::string NodeToJsonString() const;
		PLUGIFY_CONFIGS_API plg::string RootToJsonString() const;

	public:
		class Detail;

	private:
		friend Config::Detail;

		Config() = delete;
		Config(std::unique_ptr<Detail> detail);

	private:
		std::unique_ptr<Detail> _detail;
	};
}
