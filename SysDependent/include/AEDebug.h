#ifndef AEDEBUG_H_
#define AEDEBUG_H_

#include <string>
#include <sstream>

namespace aengine
{
	void AEPrintLog(std::string str);

	class dbgout_stream: public std::stringstream
	{
	public:
		~dbgout_stream(void)
		{
			AEPrintLog(this->str());
		}
	};

	// extern dbgout_stream dbgout;
	typedef dbgout_stream dbgout;
}

#endif /* AEDEBUG_H_ */