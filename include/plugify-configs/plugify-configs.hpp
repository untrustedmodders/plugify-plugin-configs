#pragma once
#include "methods.hpp"
#include <plugify_configs_export.h>
#include <string_view>
#include <memory>

namespace pcf
{
	enum cfobject_t { cfobject };
	enum cfarray_t { cfarray };

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

		PLUGIFY_CONFIGS_API void SetNull();
		PLUGIFY_CONFIGS_API void SetObject();
		PLUGIFY_CONFIGS_API void SetArray();

		inline void Set(nullptr_t) { SetNull(); };
		PLUGIFY_CONFIGS_API void Set(bool value);
		PLUGIFY_CONFIGS_API void Set(int32_t value);
		PLUGIFY_CONFIGS_API void Set(int64_t value);
		PLUGIFY_CONFIGS_API void Set(float value);
		PLUGIFY_CONFIGS_API void Set(double value);
		PLUGIFY_CONFIGS_API void Set(plg::string value);
		inline void Set(cfobject_t) { SetObject(); };
		inline void Set(cfarray_t) { SetArray(); };

		PLUGIFY_CONFIGS_API void PushNull();
		PLUGIFY_CONFIGS_API void PushBool(bool value);
		PLUGIFY_CONFIGS_API void PushInt32(int32_t value);
		PLUGIFY_CONFIGS_API void PushInt64(int64_t value);
		PLUGIFY_CONFIGS_API void PushFloat(float value);
		PLUGIFY_CONFIGS_API void PushDouble(double value);
		PLUGIFY_CONFIGS_API void PushString(plg::string value);
		PLUGIFY_CONFIGS_API void PushObject();
		PLUGIFY_CONFIGS_API void PushArray();

		PLUGIFY_CONFIGS_API bool GetBool(bool defaultValue = {}) const;
		PLUGIFY_CONFIGS_API int32_t GetInt32(int32_t defaultValue = {}) const;
		PLUGIFY_CONFIGS_API int64_t GetInt64(int64_t defaultValue = {}) const;
		PLUGIFY_CONFIGS_API float GetFloat(float defaultValue = {}) const;
		PLUGIFY_CONFIGS_API double GetDouble(double defaultValue = {}) const;
		PLUGIFY_CONFIGS_API plg::string GetString(std::string_view defaultValue = {}) const;

		PLUGIFY_CONFIGS_API bool JumpFirst();
		PLUGIFY_CONFIGS_API bool JumpLast();
		PLUGIFY_CONFIGS_API bool JumpNext();
		PLUGIFY_CONFIGS_API bool JumpPrev();
		PLUGIFY_CONFIGS_API bool JumpKey(std::string_view key, bool create = false);
		PLUGIFY_CONFIGS_API bool JumpN(int n);
		PLUGIFY_CONFIGS_API bool JumpBack();
		PLUGIFY_CONFIGS_API void JumpRoot();

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
