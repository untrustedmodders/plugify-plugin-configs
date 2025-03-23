#pragma once
#include "methods.hpp"
#include <plugify_configs_export.h>
#include <string_view>
#include <memory>

namespace pcf
{
	enum cfobject_t { cfobject };
	enum cfarray_t { cfarray };

	constexpr std::string_view rootName = { "##root##" };
	constexpr std::string_view thisNode = { "##this##" };

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

		PLUGIFY_CONFIGS_API void Merge(const Config& other);
		PLUGIFY_CONFIGS_API void MergeMove(Config&& other);

		PLUGIFY_CONFIGS_API NodeType GetType() const;
		PLUGIFY_CONFIGS_API bool IsNull() const;
		PLUGIFY_CONFIGS_API bool IsBool() const;
		PLUGIFY_CONFIGS_API bool IsInt() const;
		PLUGIFY_CONFIGS_API bool IsFloat() const;
		PLUGIFY_CONFIGS_API bool IsString() const;
		PLUGIFY_CONFIGS_API bool IsObject() const;
		PLUGIFY_CONFIGS_API bool IsArray() const;

		PLUGIFY_CONFIGS_API void SetNull(std::string_view key);
		PLUGIFY_CONFIGS_API void SetObject(std::string_view key);
		PLUGIFY_CONFIGS_API void SetArray(std::string_view key);

		inline void Set(std::string_view key, nullptr_t) { SetNull(key); };
		PLUGIFY_CONFIGS_API void Set(std::string_view key, bool value);
		PLUGIFY_CONFIGS_API void Set(std::string_view key, int32_t value);
		PLUGIFY_CONFIGS_API void Set(std::string_view key, int64_t value);
		PLUGIFY_CONFIGS_API void Set(std::string_view key, float value);
		PLUGIFY_CONFIGS_API void Set(std::string_view key, double value);
		PLUGIFY_CONFIGS_API void Set(std::string_view key, plg::string value);
		inline void Set(std::string_view key, cfobject_t) { SetObject(key); };
		inline void Set(std::string_view key, cfarray_t) { SetArray(key); };

		PLUGIFY_CONFIGS_API bool TrySetFromBool(std::string_view key, bool value);
		PLUGIFY_CONFIGS_API bool TrySetFromInt32(std::string_view key, int32_t value);
		PLUGIFY_CONFIGS_API bool TrySetFromInt64(std::string_view key, int64_t value);
		PLUGIFY_CONFIGS_API bool TrySetFromFloat(std::string_view key, float value);
		PLUGIFY_CONFIGS_API bool TrySetFromDouble(std::string_view key, double value);
		PLUGIFY_CONFIGS_API bool TrySetFromString(std::string_view key, std::string_view value);

		PLUGIFY_CONFIGS_API void PushNull();
		PLUGIFY_CONFIGS_API void PushBool(bool value);
		PLUGIFY_CONFIGS_API void PushInt32(int32_t value);
		PLUGIFY_CONFIGS_API void PushInt64(int64_t value);
		PLUGIFY_CONFIGS_API void PushFloat(float value);
		PLUGIFY_CONFIGS_API void PushDouble(double value);
		PLUGIFY_CONFIGS_API void PushString(plg::string value);
		PLUGIFY_CONFIGS_API void PushObject();
		PLUGIFY_CONFIGS_API void PushArray();

		PLUGIFY_CONFIGS_API bool GetBool(std::string_view key = thisNode, bool defaultValue = {}) const;
		PLUGIFY_CONFIGS_API int32_t GetInt32(std::string_view key = thisNode, int32_t defaultValue = {}) const;
		PLUGIFY_CONFIGS_API int64_t GetInt64(std::string_view key = thisNode, int64_t defaultValue = {}) const;
		PLUGIFY_CONFIGS_API float GetFloat(std::string_view key = thisNode, float defaultValue = {}) const;
		PLUGIFY_CONFIGS_API double GetDouble(std::string_view key = thisNode, double defaultValue = {}) const;
		PLUGIFY_CONFIGS_API plg::string GetString(std::string_view key = thisNode, std::string_view defaultValue = {}) const;

		PLUGIFY_CONFIGS_API bool GetAsBool(std::string_view key = thisNode, bool defaultValue = {}) const;
		PLUGIFY_CONFIGS_API int32_t GetAsInt32(std::string_view key = thisNode, int32_t defaultValue = {}) const;
		PLUGIFY_CONFIGS_API int64_t GetAsInt64(std::string_view key = thisNode, int64_t defaultValue = {}) const;
		PLUGIFY_CONFIGS_API float GetAsFloat(std::string_view key = thisNode, float defaultValue = {}) const;
		PLUGIFY_CONFIGS_API double GetAsDouble(std::string_view key = thisNode, double defaultValue = {}) const;
		PLUGIFY_CONFIGS_API plg::string GetAsString(std::string_view key = thisNode, std::string_view defaultValue = {}) const;

		PLUGIFY_CONFIGS_API bool HasKey(std::string_view key) const;
		PLUGIFY_CONFIGS_API bool Empty() const;
		PLUGIFY_CONFIGS_API int64_t GetSize() const;
		PLUGIFY_CONFIGS_API plg::string GetName() const;
		PLUGIFY_CONFIGS_API plg::string GetPath() const;

		PLUGIFY_CONFIGS_API bool JumpFirst();
		PLUGIFY_CONFIGS_API bool JumpLast();
		PLUGIFY_CONFIGS_API bool JumpNext();
		PLUGIFY_CONFIGS_API bool JumpPrev();
		PLUGIFY_CONFIGS_API bool JumpKey(std::string_view key, bool create = false);
		PLUGIFY_CONFIGS_API bool JumpN(int n);
		PLUGIFY_CONFIGS_API bool JumpBack();
		PLUGIFY_CONFIGS_API void JumpRoot();

		PLUGIFY_CONFIGS_API int Remove();
		PLUGIFY_CONFIGS_API bool RemoveKey(std::string_view key);
		PLUGIFY_CONFIGS_API void Clear();

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
