#ifndef CONTROL_H
#define CONTROL_H

class Control{
public:
	static Control& GetInstance();

	void Start();

	void Stop();

private:
	Control();

	bool exit_;

	//Pipe pipe_;

	// Disallow copy and assign
	Control(const Control&) = delete;
	Control& operator=(const Control&) = delete;
};
#endif
