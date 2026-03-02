#include "json_glaze.hpp"

#include <glaze/glaze.hpp>

#if __has_include(<glaze/yaml.hpp>)
#include <glaze/yaml.hpp>
#include <glaze/toml.hpp>
#endif

#include <plugify-configs/methods.hpp>
#include <plugify-configs/plugify-configs.hpp>

namespace pcf {
#if __has_include(<glaze/yaml.hpp>)
	using Value = glz::generic;
#else
	using Value = glz::json_t;
#endif

	static void Map(Value& value, Config& config) {
		if (value.is_null()) {
			config.Set(thisNode, nullptr);
		} else if (value.is_boolean()) {
			config.Set(thisNode, value.get_boolean());
		} else if (value.is_number()) {
			config.Set(thisNode, value.get_number());
		} else if (value.is_string()) {
			config.Set(thisNode, value.get_string());
		} else if (value.is_object()) {
			config.Set(thisNode, cfobject);
			auto& obj = value.get_object();
			for (auto& field: obj) {
				config.JumpKey(std::get<0>(field), true);
				Map(std::get<1>(field), config);
				config.JumpBack();
			}
		} else if (value.is_array()) {
			config.Set(thisNode, cfarray);
			auto& arr = value.get_array();
			for (auto& item: arr) {
				config.PushNull();
				config.JumpLast();
				Map(item, config);
				config.JumpBack();
			}
		} else {
			config.Set(thisNode, nullptr);
		}
	}

	std::unique_ptr<Config> ReadJsonConfig(std::string_view path) {
		Value value{};
		if (auto ec = glz::read_file_json(value, path, std::string{})) {
			SetError(glz::format_error(ec));
			return nullptr;
		}
		auto config = MakeConfig();
		Map(value, *config);
		return config;
	}

	std::unique_ptr<Config> ReadJsoncConfig(std::string_view path) {
		Value value{};
		if (auto ec = glz::read_file_jsonc(value, path, std::string{})) {
			SetError(glz::format_error(ec));
			return nullptr;
		}
		auto config = MakeConfig();
		Map(value, *config);
		return config;
	}

#if __has_include(<glaze/yaml.hpp>)
	std::unique_ptr<Config> ReadYamlConfig(std::string_view path) {
		Value value{};
		if (auto ec = glz::read_file_yaml(value, path)) {
			SetError(glz::format_error(ec));
			return nullptr;
		}
		auto config = MakeConfig();
		Map(value, *config);
		return config;
	}

	std::unique_ptr<Config> ReadTomlConfig(std::string_view path) {
		Value value{};
		if (auto ec = glz::read_file_toml(value, path, std::string{})) {
			SetError(glz::format_error(ec));
			return nullptr;
		}
		auto config = MakeConfig();
		Map(value, *config);
		return config;
	}
#endif
}// namespace pcf
