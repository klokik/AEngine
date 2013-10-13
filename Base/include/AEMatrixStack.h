/*
 * AEMatrixStack.h
 *
 *  Created on: Jun 19, 2012
 *      Author: klokik
 */

#ifndef AEMATRIXSTACK_H_
#define AEMATRIXSTACK_H_

#include <stack>

#include "AEMatrix4f4.h"


namespace aengine
{
	class AEMatrixStack: public AEMatrix4f4
	{
	protected:
		std::stack<AEMatrix4f4> m_stack;
	public:
		AEMatrixStack(void);

		void PushMatrix(void);
		void PopMatrix(void);
		size_t GetDepth(void);
		void Clear(void);
		bool IsEmpty(void);

		using AEMatrix4f4::operator=;

		AEMatrix4f4 &GetMatrix(void);
		void SetMatrix(AEMatrix4f4 const &val);

		virtual ~AEMatrixStack(void);
	};
}

#endif /* AEMATRIXSTACK_H_ */
