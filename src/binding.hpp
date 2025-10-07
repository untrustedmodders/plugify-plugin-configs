#pragma once

#include <plugify_configs_export.h>
#include <plg/string.hpp>
#include <plg/vector.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration
namespace pcf {
	class Config;
}

// Node type enum matching C++ enum
typedef enum {
    PLG_CONFIG_NODE_NULL = 0,
    PLG_CONFIG_NODE_BOOL,
    PLG_CONFIG_NODE_INT,
    PLG_CONFIG_NODE_FLOAT,
    PLG_CONFIG_NODE_STRING,
    PLG_CONFIG_NODE_OBJECT,
    PLG_CONFIG_NODE_ARRAY
} NodeType;

// Config creation and destruction
PLUGIFY_CONFIGS_API pcf::Config* Read(const plg::string& path);
PLUGIFY_CONFIGS_API pcf::Config* ReadMultiple(const plg::vector<plg::string>& paths);
PLUGIFY_CONFIGS_API pcf::Config* Make();
PLUGIFY_CONFIGS_API void Delete(pcf::Config* config);

// Error handling
PLUGIFY_CONFIGS_API void SetError(const plg::string& error);
PLUGIFY_CONFIGS_API plg::string GetError();

// Merge operations
PLUGIFY_CONFIGS_API void Merge(pcf::Config* config, pcf::Config* other);
PLUGIFY_CONFIGS_API void MergeMove(pcf::Config* config, pcf::Config* other);

// Type checking
PLUGIFY_CONFIGS_API NodeType GetType(pcf::Config* config);
PLUGIFY_CONFIGS_API bool IsNull(pcf::Config* config);
PLUGIFY_CONFIGS_API bool IsBool(pcf::Config* config);
PLUGIFY_CONFIGS_API bool IsInt(pcf::Config* config);
PLUGIFY_CONFIGS_API bool IsFloat(pcf::Config* config);
PLUGIFY_CONFIGS_API bool IsString(pcf::Config* config);
PLUGIFY_CONFIGS_API bool IsObject(pcf::Config* config);
PLUGIFY_CONFIGS_API bool IsArray(pcf::Config* config);

// Set methods
PLUGIFY_CONFIGS_API void SetNull(pcf::Config* config, const plg::string& key);
PLUGIFY_CONFIGS_API void SetObject(pcf::Config* config, const plg::string& key);
PLUGIFY_CONFIGS_API void SetArray(pcf::Config* config, const plg::string& key);
PLUGIFY_CONFIGS_API void SetBool(pcf::Config* config, const plg::string& key, bool value);
PLUGIFY_CONFIGS_API void SetInt(pcf::Config* config, const plg::string& key, int64_t value);
PLUGIFY_CONFIGS_API void SetFloat(pcf::Config* config, const plg::string& key, double value);
PLUGIFY_CONFIGS_API void SetString(pcf::Config* config, const plg::string& key, const plg::string& value);

// TrySet methods
PLUGIFY_CONFIGS_API bool TrySetFromBool(pcf::Config* config, const plg::string& key, bool value);
PLUGIFY_CONFIGS_API bool TrySetFromInt(pcf::Config* config, const plg::string& key, int64_t value);
PLUGIFY_CONFIGS_API bool TrySetFromFloat(pcf::Config* config, const plg::string& key, double value);
PLUGIFY_CONFIGS_API bool TrySetFromString(pcf::Config* config, const plg::string& key, const plg::string& value);

// Push methods for arrays
PLUGIFY_CONFIGS_API void PushNull(pcf::Config* config);
PLUGIFY_CONFIGS_API void PushBool(pcf::Config* config, bool value);
PLUGIFY_CONFIGS_API void PushInt(pcf::Config* config, int64_t value);
PLUGIFY_CONFIGS_API void PushFloat(pcf::Config* config, double value);
PLUGIFY_CONFIGS_API void PushString(pcf::Config* config, const plg::string& value);
PLUGIFY_CONFIGS_API void PushObject(pcf::Config* config);
PLUGIFY_CONFIGS_API void PushArray(pcf::Config* config);

// Get methods
PLUGIFY_CONFIGS_API bool GetBool(pcf::Config* config, const plg::string& key, bool defaultValue);
PLUGIFY_CONFIGS_API int64_t GetInt(pcf::Config* config, const plg::string& key, int64_t defaultValue);
PLUGIFY_CONFIGS_API double GetFloat(pcf::Config* config, const plg::string& key, double defaultValue);
PLUGIFY_CONFIGS_API plg::string GetString(pcf::Config* config, const plg::string& key, const plg::string& defaultValue);

// GetAs methods
PLUGIFY_CONFIGS_API bool GetAsBool(pcf::Config* config, const plg::string& key, bool defaultValue);
PLUGIFY_CONFIGS_API int64_t GetAsInt(pcf::Config* config, const plg::string& key, int64_t defaultValue);
PLUGIFY_CONFIGS_API double GetAsFloat(pcf::Config* config, const plg::string& key, double defaultValue);
PLUGIFY_CONFIGS_API plg::string GetAsString(pcf::Config* config, const plg::string& key, const plg::string& defaultValue);

// Query methods
PLUGIFY_CONFIGS_API bool HasKey(pcf::Config* config, const plg::string& key);
PLUGIFY_CONFIGS_API bool Empty(pcf::Config* config);
PLUGIFY_CONFIGS_API int64_t GetSize(pcf::Config* config);
PLUGIFY_CONFIGS_API plg::string GetName(pcf::Config* config);
PLUGIFY_CONFIGS_API plg::string GetPath(pcf::Config* config);

// Navigation methods
PLUGIFY_CONFIGS_API bool JumpFirst(pcf::Config* config);
PLUGIFY_CONFIGS_API bool JumpLast(pcf::Config* config);
PLUGIFY_CONFIGS_API bool JumpNext(pcf::Config* config);
PLUGIFY_CONFIGS_API bool JumpPrev(pcf::Config* config);
PLUGIFY_CONFIGS_API bool JumpKey(pcf::Config* config, const plg::string& key, bool create);
PLUGIFY_CONFIGS_API bool JumpN(pcf::Config* config, int n);
PLUGIFY_CONFIGS_API bool JumpBack(pcf::Config* config);
PLUGIFY_CONFIGS_API void JumpRoot(pcf::Config* config);

// Removal methods
PLUGIFY_CONFIGS_API int Remove(pcf::Config* config);
PLUGIFY_CONFIGS_API bool RemoveKey(pcf::Config* config, const plg::string& key);
PLUGIFY_CONFIGS_API void Clear(pcf::Config* config);

// JSON export
PLUGIFY_CONFIGS_API plg::string NodeToJsonString(pcf::Config* config);
PLUGIFY_CONFIGS_API plg::string RootToJsonString(pcf::Config* config);

#ifdef __cplusplus
}
#endif
