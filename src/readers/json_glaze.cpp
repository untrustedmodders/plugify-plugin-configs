#include <plugify-configs/methods.hpp>
#include <plugify-configs/plugify-configs.hpp>
#include <string_view>
#include <memory>

namespace pcf
{
	std::unique_ptr<Config> ReadJsonConfig([[maybe_unused]] std::string_view path)
	{
		auto config = MakeConfig();
		return config;
	}
}
