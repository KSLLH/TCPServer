#ifndef SERVICE_H
#define SERVICE_H

class ServiceManager{
public:
	static ServiceManager& GetInstance();
	
	void Start();

private:
	ServiceManager();
};
#endif
