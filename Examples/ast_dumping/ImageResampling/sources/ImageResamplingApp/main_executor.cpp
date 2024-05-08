#include "main_executor.h"
#include "image_resampler.h"
#include "bilinear_interpolation.h"
#include "bicubic_interpolation.h"
#include "lanczos_interpolation.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <sstream>

namespace
{
	enum args_indices
	{
		name_of_executable [[maybe_unused]],
		image_path,
		save_path,
		resampled_width,
		resampled_height,
		method,
		additional_lanczos_param
	};

	constexpr int min_parameters_required = method;

	void print_help()
	{
		std::cout << " This is the application for images resampling using different interpolation methods.\n";
		std::cout << " You can increase the resolution as well as reduce it.\n";
		std::cout << "\n For all methods following parameters are required\n (set all parameters separated with space without names of parameters and other symbols):\n";
		std::cout << " \t1. Path (with filename) to the image which will be resampled;\n";
		std::cout << " \t2. Path (with filename) the resampled image must be save to. Path must be free;\n";
		std::cout << " \t3. Resampled width of the image: integer positive number;\n";
		std::cout << " \t4. Resampled height of the image: integer positive number;\n";
		std::cout << " \t5. Interpolation method. Available methods: bilinear, bicubic, lanczos.\n";
		std::cout << "\n \tAdditional. If you chose the Lanczos's interpolation pass the additional param\n which denotes the furthest considerable pixel from the interpolated one.\n\n";
		std::cout << " Enjoy the application!\n";
	}

	void argv_validation(const char** argv)
	{
		if (argv == nullptr)
		{
			throw std::invalid_argument("argv can not be nullptr.\n");
		}
	}

	bool argc_validation(const int argc, const char** argv)
	{
		argv_validation(argv);
		if (argc < 1)
		{
			throw std::exception("argc can not be less than 1.\n");
		}
		if (argc == 1 || std::string(argv[1]) == "-h")
		{
			print_help();
			return false;
		}

		if (argc < min_parameters_required)
		{
			std::cout << "Not enough args_indices. At least " << min_parameters_required << " are required.\n";
			return false;
		}
		return true;
	}

	bool pathes_validation(const std::string& image_path, const std::string& save_path)
	{
		if (!std::filesystem::exists(image_path))
		{
			std::cout << "File " << image_path << " wasn't found. Try to change the path.\n";
			return false;
		}
		if (std::filesystem::exists(save_path))
		{
			std::cout << "File " << save_path << " exists already. Change the name.\n";
			return false;
		}

		if (image_path.size() < 4 || image_path.substr(image_path.size() - 4) != ".bmp" ||
			save_path.size() < 4 || save_path.substr(save_path.size() - 4) != ".bmp")
		{
			std::cout << "Only .bmp format of images is available\n";
			return false;
		}
		return true;
	}

	std::int32_t get_resampled_side(const std::string& int_as_str)
	{
		std::int32_t integer_value{};
		std::istringstream value_iss(int_as_str);
		value_iss >> integer_value;
		if (integer_value < 1)
		{
			throw std::invalid_argument("Invalid resampled side.\n");
		}
		return integer_value;
	}

	const point_on_image_abstract_interpolation* choose_interpolation(const int argc, const char** argv)
	{
		argv_validation(argv);
		if (argc <= method)
		{
			throw std::exception("Few parameters. Method wasn't passed.\n");
		}
		if (std::string(argv[method]) == "bilinear")
		{
			return new bilinear_interpolation();
		}

		if (std::string(argv[method]) == "bicubic")
		{
			return new bicubic_interpolation();
		}

		if (std::string(argv[method]) == "lanczos")
		{
			if (argc <= additional_lanczos_param)
			{
				throw std::exception("No additional param for Lanczos's interpolation.\n");
			}
			double a{};
			std::istringstream a_iss(argv[additional_lanczos_param]);
			a_iss >> a;
			if (a < 1)
			{
				throw std::invalid_argument("Invalid additional param for Lanczos's interpolation.\n");
			}
			return new lanczos_interpolation(a);
		}

		throw std::invalid_argument("Non-existent interpolation method.\n");
	}
}

int main_executor::execute(const int argc, const char** argv)
{
	argv_validation(argv);
	if (!argc_validation(argc, argv))
	{
		return 1;
	}

	const std::string image_path(argv[args_indices::image_path]);
	const std::string save_path(argv[args_indices::save_path]);
	if (!pathes_validation(image_path, save_path))
	{
		return 1;
	}

	const std::int32_t resampled_width = get_resampled_side(argv[args_indices::resampled_width]);
	const std::int32_t resampled_height = get_resampled_side(argv[args_indices::resampled_height]);

	const auto* interpolation_ptr = choose_interpolation(argc, argv);
	try
	{
		const bmp::Bitmap initial_image(image_path);
		bmp::Bitmap new_image{ image_resampler::resample(initial_image, resampled_width, resampled_height, *interpolation_ptr) };
		new_image.save(save_path);
		delete interpolation_ptr;
	}
	catch (std::exception& ex)
	{
		std::cout << ex.what() << "\nTry again.\n";
		delete interpolation_ptr;
		return 1;
	}

	std::cout << "Resampling was performed successfully!\n";
	return 0;
}
