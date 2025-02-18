#include <plugify-configs/methods.hpp>
#include <plugify-configs/plugify-configs.hpp>
#include <glaze/glaze.hpp>
#include <string_view>
#include <memory>

namespace pcf
{
	static void MapJson(glz::json_t& json, Config& config)
	{
		if (json.is_null()) {
			config.Set(nullptr);
		}
		else if (json.is_boolean()) {
			config.Set(json.get_boolean());
		}
		else if (json.is_number()) {
			config.Set(json.get_number());
		}
		else if (json.is_string()) {
			config.Set(json.get_string());
		}
		else if (json.is_object()) {
			config.Set(cfobject);
			auto& obj = json.get_object();
			for (auto& field : obj) {
				config.JumpKey(std::get<0>(field), true);
				MapJson(std::get<1>(field), config);
				config.JumpBack();
			}
		}
		else if (json.is_array()) {
			config.Set(cfarray);
			auto& arr = json.get_array();
			for (auto& item : arr) {
				config.PushNull();
				config.JumpLast();
				MapJson(item, config);
				config.JumpBack();
			}
		}
		else {
			config.Set(nullptr);
		}
	}

	std::unique_ptr<Config> ReadJsonConfig(std::string_view path)
	{
		glz::json_t json{};
		auto ec = glz::read_file_json(json, path, std::string{});
		if (ec) {
			SetError(glz::format_error(ec));
			return nullptr;
		}
		auto config = MakeConfig();
		MapJson(json, *config);
		return config;
	}
}
