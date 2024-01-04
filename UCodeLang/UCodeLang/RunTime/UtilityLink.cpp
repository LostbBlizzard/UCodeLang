#include "UtilityLink.hpp"
#include "Interpreters/Interpreter.hpp"


#include <cmath>
#include <mutex>
UCodeLangStart

void UtilityLink::Link(RunTimeLib& lib, const LinkSettings& settings)
{
	if (settings.AddMath)
	{
		//Trigonometric functions
		//https://cplusplus.com/reference/cmath/

		//Trigonometric functions
		lib.Add_CPPCall<float>("cosf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::cos(Par0));
			}, std::cos);

		lib.Add_CPPCall<float64>("cos", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::cos(Par0));
			}, std::cos);



		lib.Add_CPPCall<float>("sinf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::sin(Par0));
			}, std::sin);

		lib.Add_CPPCall<float64>("sin", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::sin(Par0));
			}, std::sin);



		lib.Add_CPPCall<float>("tanf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::tan(Par0));
			}, std::tan);

		lib.Add_CPPCall<float64>("tan", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::tan(Par0));
			}, std::tan);


		lib.Add_CPPCall<float>("acosf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::acos(Par0));
			}, std::acos);

		lib.Add_CPPCall<float64>("acos", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::acos(Par0));
			}, std::acos);


		lib.Add_CPPCall<float>("asinf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::asin(Par0));
			}, std::asin);

		lib.Add_CPPCall<float64>("asins", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::asin(Par0));
			}, std::asin);


		lib.Add_CPPCall<float>("atanf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::atan(Par0));
			}, std::atan);

		lib.Add_CPPCall<float64>("atan", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::atan(Par0));
			}, std::atan);


		lib.Add_CPPCall<float>("atan2f", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				auto Par1 = Input.GetParameter<float>();
				Input.Set_Return(std::atan2(Par0, Par1));
			}, std::atan2);

		lib.Add_CPPCall<float64>("atan", [](InterpreterCPPinterface& Input)
			{

				auto Par0 = Input.GetParameter<float64>();
				auto Par1 = Input.GetParameter<float64>();
				Input.Set_Return(std::atan2(Par0, Par1));
			}, std::atan2);

		//Hyperbolic functions

		lib.Add_CPPCall<float>("coshf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::cosh(Par0));
			}, std::cosh);

		lib.Add_CPPCall<float64>("cosh", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::cosh(Par0));
			}, std::cosh);



		lib.Add_CPPCall<float>("sinhf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::sinh(Par0));
			}, std::sinh);

		lib.Add_CPPCall<float64>("sinh", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::sinh(Par0));
			}, std::sinh);


		lib.Add_CPPCall<float>("tanhf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::tanh(Par0));
			}, std::tanh);

		lib.Add_CPPCall<float64>("tanh", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::tanh(Par0));
			}, std::tanh);


		lib.Add_CPPCall<float>("acoshf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::acosh(Par0));
			}, std::acosh);

		lib.Add_CPPCall<float64>("acosh", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::acosh(Par0));
			}, std::acosh);


		lib.Add_CPPCall<float>("acoshf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::asinh(Par0));
			}, std::asinh);

		lib.Add_CPPCall<float64>("asinh", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::asinh(Par0));
			}, std::asinh);

		lib.Add_CPPCall<float>("atanhf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::atanh(Par0));
			}, std::atanh);

		lib.Add_CPPCall<float64>("atanh", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::atanh(Par0));
			}, std::atanh);


		//Exponential and logarithmic functions
		lib.Add_CPPCall<float>("expf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::exp(Par0));
			}, std::exp);

		lib.Add_CPPCall<float64>("exp", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::exp(Par0));
			}, std::exp);

		lib.Add_CPPCall<float>("frexpf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::exp(Par0));
			}, std::exp);

		lib.Add_CPPCall<float64>("frexp", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::exp(Par0));
			}, std::exp);

		lib.Add_CPPCall<float>("ldexpf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				auto Par1 = Input.GetParameter<int>();
				Input.Set_Return(std::ldexp(Par0, Par1));
			}, std::ldexp);

		lib.Add_CPPCall<float64>("ldexp", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				auto Par1 = Input.GetParameter<int>();
				Input.Set_Return(std::ldexp(Par0, Par1));
			}, std::ldexp);

		lib.Add_CPPCall<float>("logf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::log(Par0));
			}, std::log);

		lib.Add_CPPCall<float64>("log", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::log(Par0));
			}, std::log);

		lib.Add_CPPCall<float>("log10f", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::log10(Par0));
			}, std::log10);

		lib.Add_CPPCall<float64>("log10", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::log10(Par0));
			}, std::log10);

		lib.Add_CPPCall<float>("modf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				auto Par1 = Input.GetParameter<float*>();
				Input.Set_Return(std::modf(Par0, Par1));
			}, std::modf);

		lib.Add_CPPCall<float64>("modff", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				auto Par1 = Input.GetParameter<float64*>();
				Input.Set_Return(std::modf(Par0, Par1));
			}, std::modf);

		lib.Add_CPPCall<float>("exp2f", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::exp2(Par0));
			}, std::exp2);

		lib.Add_CPPCall<float64>("exp2", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::exp2(Par0));
			}, std::exp2);

		lib.Add_CPPCall<float>("expm1f", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::expm1(Par0));
			}, std::expm1);

		lib.Add_CPPCall<float64>("expm1", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::expm1(Par0));
			}, std::expm1);

		lib.Add_CPPCall<int, float>("ilogbf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::ilogb(Par0));
			}, std::ilogb);

		lib.Add_CPPCall<int,float64>("ilogb", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::ilogb(Par0));
			}, std::ilogb);

		lib.Add_CPPCall<float>("log1pf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::log1p(Par0));
			}, std::log1p);

		lib.Add_CPPCall<float64>("log1p", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::log1p(Par0));
			}, std::log1p);

		lib.Add_CPPCall<float>("log2f", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::log2(Par0));
			}, std::log2);

		lib.Add_CPPCall<float64>("log2", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::log2(Par0));
			}, std::log2);

		lib.Add_CPPCall<float>("logbf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::logb(Par0));
			}, std::logb);

		lib.Add_CPPCall<float64>("logb", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::logb(Par0));
			}, std::logb);

		lib.Add_CPPCall<float>("scalbnf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				auto Par1 = Input.GetParameter<int>();
				Input.Set_Return(std::scalbn(Par0, Par1));
			}, std::scalbn);

		lib.Add_CPPCall<float64>("scalbn", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				auto Par1 = Input.GetParameter<int>();
				Input.Set_Return(std::scalbn(Par0, Par1));
			}, std::scalbn);

		lib.Add_CPPCall<float>("scalblnf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				auto Par1 = Input.GetParameter<int>();
				Input.Set_Return(std::scalbln(Par0, Par1));
			}, std::scalbln);

		lib.Add_CPPCall<float64>("scalbln", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				auto Par1 = Input.GetParameter<int>();
				Input.Set_Return(std::scalbln(Par0, Par1));
			}, std::scalbln);

		//Power functions
		lib.Add_CPPCall<float>("powf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				auto Par1 = Input.GetParameter<float>();
				Input.Set_Return(std::pow(Par0, Par1));
			}, std::pow);

		lib.Add_CPPCall<float64>("pow", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				auto Par1 = Input.GetParameter<float64>();
				Input.Set_Return(std::pow(Par0, Par1));
			}, std::pow);

		lib.Add_CPPCall<float>("sqrtf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::sqrt(Par0));
			}, std::sqrt);

		lib.Add_CPPCall<float64>("sqrt", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::sqrt(Par0));
			}, std::sqrt);

		lib.Add_CPPCall<float>("cbrtf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				Input.Set_Return(std::cbrt(Par0));
			}, std::cbrt);

		lib.Add_CPPCall<float64>("cbrt", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				Input.Set_Return(std::cbrt(Par0));
			}, std::cbrt);

		lib.Add_CPPCall<float,float,float>("hypotf", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float>();
				auto Par1 = Input.GetParameter<float>();
				Input.Set_Return(std::hypot(Par0, Par1));
			}, std::hypot);

		lib.Add_CPPCall<float64, float64, float64>("hypot", [](InterpreterCPPinterface& Input)
			{
				auto Par0 = Input.GetParameter<float64>();
				auto Par1 = Input.GetParameter<float64>();
				Input.Set_Return(std::hypot(Par0, Par1));
			}, std::hypot);
		//Error and gamma functions

		//Rounding and remainder functions
	}
	if (settings.AddSIMD)
	{

	}
	if (settings.AddThreading)
	{
		
	}
}

UCodeLangEnd


