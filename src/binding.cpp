#include "binding.hpp"
#include "config.hpp"

using namespace pcf;

// Config creation and destruction
PLUGIFY_CONFIGS_API Config* Read(const plg::string& path) {
    auto config = pcf::ReadConfig(path);
    return config.release();
}

PLUGIFY_CONFIGS_API Config* ReadMultiple(const plg::vector<plg::string>& paths) {
    std::vector<std::string_view> pathViews;
    pathViews.reserve(paths.size());
    for (const auto& path : paths) {
        pathViews.emplace_back(path);
    }

    auto config = pcf::ReadConfigs(pathViews);
    return config.release();
}

PLUGIFY_CONFIGS_API Config* Make() {
    auto config = pcf::MakeConfig();
    return config.release();
}

PLUGIFY_CONFIGS_API void Delete(Config* config) {
    delete config;
}

// Error handling
PLUGIFY_CONFIGS_API void SetError(const plg::string& error) {
    pcf::SetError(error);
}

PLUGIFY_CONFIGS_API plg::string GetError() {
    return plg::string(pcf::GetError());
}

// Merge operations
PLUGIFY_CONFIGS_API void Merge(Config* config, Config* other) {
    if (config && other) {
        config->Merge(*other);
    }
}

PLUGIFY_CONFIGS_API void MergeMove(Config* config, Config* other) {
    if (config && other) {
        config->MergeMove(std::move(*other));
    }
}

// Type checking
PLUGIFY_CONFIGS_API NodeType GetType(Config* config) {
    if (!config) return PLG_CONFIG_NODE_NULL;
    return static_cast<NodeType>(config->GetType());
}

PLUGIFY_CONFIGS_API bool IsNull(Config* config) {
    return config ? config->IsNull() : true;
}

PLUGIFY_CONFIGS_API bool IsBool(Config* config) {
    return config ? config->IsBool() : false;
}

PLUGIFY_CONFIGS_API bool IsInt(Config* config) {
    return config ? config->IsInt() : false;
}

PLUGIFY_CONFIGS_API bool IsFloat(Config* config) {
    return config ? config->IsFloat() : false;
}

PLUGIFY_CONFIGS_API bool IsString(Config* config) {
    return config ? config->IsString() : false;
}

PLUGIFY_CONFIGS_API bool IsObject(Config* config) {
    return config ? config->IsObject() : false;
}

PLUGIFY_CONFIGS_API bool IsArray(Config* config) {
    return config ? config->IsArray() : false;
}

// Set methods
PLUGIFY_CONFIGS_API void SetNull(Config* config, const plg::string& key) {
    if (config) {
        config->SetNull(key);
    }
}

PLUGIFY_CONFIGS_API void SetObject(Config* config, const plg::string& key) {
    if (config) {
        config->SetObject(key);
    }
}

PLUGIFY_CONFIGS_API void SetArray(Config* config, const plg::string& key) {
    if (config) {
        config->SetArray(key);
    }
}

PLUGIFY_CONFIGS_API void SetBool(Config* config, const plg::string& key, bool value) {
    if (config) {
        config->Set(key, value);
    }
}

PLUGIFY_CONFIGS_API void SetInt(Config* config, const plg::string& key, int64_t value) {
    if (config) {
        config->Set(key, value);
    }
}

PLUGIFY_CONFIGS_API void SetFloat(Config* config, const plg::string& key, double value) {
    if (config) {
        config->Set(key, value);
    }
}

PLUGIFY_CONFIGS_API void SetString(Config* config, const plg::string& key, const plg::string& value) {
    if (config) {
        config->Set(key, value);
    }
}

// TrySet methods
PLUGIFY_CONFIGS_API bool TrySetFromBool(Config* config, const plg::string& key, bool value) {
    if (!config) return false;
    return config->TrySetFromBool(key, value);
}

PLUGIFY_CONFIGS_API bool TrySetFromInt(Config* config, const plg::string& key, int64_t value) {
    if (!config) return false;
    return config->TrySetFromInt64(key, value);
}

PLUGIFY_CONFIGS_API bool TrySetFromFloat(Config* config, const plg::string& key, double value) {
    if (!config) return false;
    return config->TrySetFromDouble(key, value);
}

PLUGIFY_CONFIGS_API bool TrySetFromString(Config* config, const plg::string& key, const plg::string& value) {
    if (!config) return false;
    return config->TrySetFromString(key, value);
}

// Push methods for arrays
PLUGIFY_CONFIGS_API void PushNull(Config* config) {
    if (config) {
        config->PushNull();
    }
}

PLUGIFY_CONFIGS_API void PushBool(Config* config, bool value) {
    if (config) {
        config->PushBool(value);
    }
}

PLUGIFY_CONFIGS_API void PushInt(Config* config, int64_t value) {
    if (config) {
        config->PushInt64(value);
    }
}

PLUGIFY_CONFIGS_API void PushFloat(Config* config, double value) {
    if (config) {
        config->PushDouble(value);
    }
}

PLUGIFY_CONFIGS_API void PushString(Config* config, plg::string value) {
    if (config) {
        config->PushString(std::move(value));
    }
}

PLUGIFY_CONFIGS_API void PushObject(Config* config) {
    if (config) {
        config->PushObject();
    }
}

PLUGIFY_CONFIGS_API void PushArray(Config* config) {
    if (config) {
        config->PushArray();
    }
}

// Get methods
PLUGIFY_CONFIGS_API bool GetBool(Config* config, const plg::string& key, bool defaultValue) {
    if (!config) return defaultValue;
    return config->GetBool(key, defaultValue);
}

PLUGIFY_CONFIGS_API int64_t GetInt(Config* config, const plg::string& key, int64_t defaultValue) {
    if (!config) return defaultValue;
    return config->GetInt64(key, defaultValue);
}

PLUGIFY_CONFIGS_API double GetFloat(Config* config, const plg::string& key, double defaultValue) {
    if (!config) return defaultValue;
    return config->GetDouble(key, defaultValue);
}

PLUGIFY_CONFIGS_API plg::string GetString(Config* config, const plg::string& key, const plg::string& defaultValue) {
    if (!config) return defaultValue;
    return config->GetString(key, defaultValue);
}

// GetAs methods
PLUGIFY_CONFIGS_API bool GetAsBool(Config* config, const plg::string& key, bool defaultValue) {
    if (!config) return defaultValue;
    return config->GetAsBool(key, defaultValue);
}

PLUGIFY_CONFIGS_API int64_t GetAsInt(Config* config, const plg::string& key, int64_t defaultValue) {
    if (!config) return defaultValue;
    return config->GetAsInt64(key, defaultValue);
}

PLUGIFY_CONFIGS_API double GetAsFloat(Config* config, const plg::string& key, double defaultValue) {
    if (!config) return defaultValue;
    return config->GetAsDouble(key, defaultValue);
}

PLUGIFY_CONFIGS_API plg::string GetAsString(Config* config, const plg::string& key, const plg::string& defaultValue) {
    if (!config) return defaultValue;
    return config->GetAsString(key, defaultValue);
}

// Query methods
PLUGIFY_CONFIGS_API bool HasKey(Config* config, const plg::string& key) {
    if (!config) return false;
    return config->HasKey(key);
}

PLUGIFY_CONFIGS_API bool Empty(Config* config) {
    if (!config) return true;
    return config->Empty();
}

PLUGIFY_CONFIGS_API int64_t GetSize(Config* config) {
    if (!config) return 0;
    return config->GetSize();
}

PLUGIFY_CONFIGS_API plg::string GetName(Config* config) {
    if (!config) return plg::string();
    return config->GetName();
}

PLUGIFY_CONFIGS_API plg::string GetPath(Config* config) {
    if (!config) return plg::string();
    return config->GetPath();
}

// Navigation methods
PLUGIFY_CONFIGS_API bool JumpFirst(Config* config) {
    if (!config) return false;
    return config->JumpFirst();
}

PLUGIFY_CONFIGS_API bool JumpLast(Config* config) {
    if (!config) return false;
    return config->JumpLast();
}

PLUGIFY_CONFIGS_API bool JumpNext(Config* config) {
    if (!config) return false;
    return config->JumpNext();
}

PLUGIFY_CONFIGS_API bool JumpPrev(Config* config) {
    if (!config) return false;
    return config->JumpPrev();
}

PLUGIFY_CONFIGS_API bool JumpKey(Config* config, const plg::string& key, bool create) {
    if (!config) return false;
    return config->JumpKey(key, create);
}

PLUGIFY_CONFIGS_API bool JumpN(Config* config, int n) {
    if (!config) return false;
    return config->JumpN(n);
}

PLUGIFY_CONFIGS_API bool JumpBack(Config* config) {
    if (!config) return false;
    return config->JumpBack();
}

PLUGIFY_CONFIGS_API void JumpRoot(Config* config) {
    if (config) {
        config->JumpRoot();
    }
}

// Removal methods
PLUGIFY_CONFIGS_API int Remove(Config* config) {
    if (!config) return 0;
    return config->Remove();
}

PLUGIFY_CONFIGS_API bool RemoveKey(Config* config, const plg::string& key) {
    if (!config) return false;
    return config->RemoveKey(key);
}

PLUGIFY_CONFIGS_API void Clear(Config* config) {
    if (config) {
        config->Clear();
    }
}

// JSON export
PLUGIFY_CONFIGS_API plg::string NodeToJsonString(Config* config) {
    if (!config) return plg::string();
    return config->NodeToJsonString();
}

PLUGIFY_CONFIGS_API plg::string RootToJsonString(Config* config) {
    if (!config) return plg::string();
    return config->RootToJsonString();
}
