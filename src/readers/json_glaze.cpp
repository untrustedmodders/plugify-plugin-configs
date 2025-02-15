#include <plugify-configs/methods.hpp>
#include <plugify-configs/plugify-configs.hpp>
#include <glaze/glaze.hpp>
#include <string_view>
#include <memory>

namespace pcf
{
	std::unique_ptr<Config> ReadJsonConfig(std::string_view path)
	{
		glz::json_t json{};
		auto ec = glz::read_file_json(json, path, std::string{});
		if (ec) {
			SetError(glz::format_error(ec));
			return nullptr;
		}
		auto config = MakeConfig();
		config->Set(444);
		return config;
	}
}
