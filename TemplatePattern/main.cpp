#include <string>
#include <iostream>
#include <Windows.h>

/*
AbstractMeasurement: Define the shared component and the main algorythm here
*/
class AbstractMeasurement {
private:
	
public:
	double meas = 0.0;

	void TemplateMethod() {
		// Algorythm capability can be extended here
		// Depending of the object passing throw, only the available methods will be called

		this->start();
		this->configure();

		this->measure(meas);
		this->filtering(meas);	// Optionnal
		this->convert_to_ohm(meas);	// Optionnal

		this->display(meas);

		this->close();
		this->stop();
	}

	// Function that measurement share
	void start(void) const{ std::cout << std::string("Starting device"); std::cout << std::endl; }
	void display(const double value) const { std::cout << std::string("result:") << value ; std::cout << std::endl;}
	void stop(void) const { std::cout << std::string("Stoping device"); std::cout << std::endl; std::cout << std::endl;}

	// Function that depend of the object but are mandatory in every object (pure virtual fonctions)
	virtual void configure(void) const = 0;
	virtual void measure(double &value) = 0;
	virtual void close(void) const = 0;

	// Optionnal function depending on the object
	virtual void filtering(double &value) {};
	virtual void convert_to_ohm(double &value) {};
};

/*
CheapOscope: Customize here what a Cheap oscope do
*/

class CheapOscope:public AbstractMeasurement {
private:
	double internal_meas = 0.0;
protected:

	// Mandatory function

	void configure(void) const override{
		std::cout << std::string("Opening Oscope remote line"); std::cout << std::endl;
		std::cout << std::string("Configuring trigger"); std::cout << std::endl;
		std::cout << std::string("Configuring probe mode x10 "); std::cout << std::endl;
		std::cout << std::string("Configuring windows"); std::cout << std::endl;
	};
	void measure(double& value) override {
		std::cout << std::string("Measurement start"); std::cout << std::endl;
		Sleep(1000); // Acquisition simulate
		internal_meas = 12.0; // Acquisition simulate
		value = internal_meas; // update reference
		std::cout << std::string("Measurement done"); std::cout << std::endl;
	};
	void close(void) const override {
		std::cout << std::string("Closing Oscope remote line"); std::cout << std::endl;
	}

	// Optionnals function

};

/*
BetterOscope: Customize here what a Better oscope do
*/
class BetterOscope :public AbstractMeasurement {
private:
	double internal_meas = 0.0;
	double internal_meas0 = 10.0; // past value for filtering mode
protected:

	// Mandatory function

	void configure(void) const override {
		std::cout << std::string("Opening Oscope remote line"); std::cout << std::endl;
		std::cout << std::string("Configuring trigger"); std::cout << std::endl;
		std::cout << std::string("Configuring probe mode x10 "); std::cout << std::endl;
		std::cout << std::string("Configuring windows"); std::cout << std::endl;
	};
	void measure(double& value) override {
		std::cout << std::string("Measurement start"); std::cout << std::endl;
		Sleep(1000); // Acquisition simulate
		internal_meas = 10.538; // Acquisition simulate
		value = internal_meas; // update reference
		std::cout << std::string("Measurement done"); std::cout << std::endl;
	};
	void close(void) const override {
		std::cout << std::string("Closing Oscope remote line"); std::cout << std::endl;
	}

	// Optionnals function

	void filtering(double& value) override {
		double a = 0.5;
		value = (value * a) + ((1 - a) * internal_meas0);	// Low pass filter EMA "f(x) = x1 * a + (1-a) * x0" where a [0;1]
	}
	void convert_to_ohm(double& value) override {
		int resistor = 22000;
		value = (value * value) / resistor; // U^2/R
	}
};

void measure(AbstractMeasurement* Algorythm)
{
	Algorythm->TemplateMethod();
}

int main(void)
{
	CheapOscope* Siglent = new CheapOscope;
	measure(Siglent);
	
	BetterOscope* Tektronix = new BetterOscope;
	measure(Tektronix);

	return EXIT_SUCCESS;
}