/*
 * AEMatrixStack.cpp
 *
 *  Created on: Jun 19, 2012
 *      Author: klokik
 */

#include "AEMatrixStack.h"

AEMatrixStack::AEMatrixStack(void)
{
}

void AEMatrixStack::PushMatrix(void)
{
	m_stack.push(*this);
}

void AEMatrixStack::PopMatrix(void)
{
	this->operator=(m_stack.top());
	m_stack.pop();
}

size_t AEMatrixStack::GetDepth(void)
{
	return this->m_stack.size();
}

void AEMatrixStack::Clear(void)
{
	while(!m_stack.empty())
	{
		m_stack.pop();
	}
}

bool AEMatrixStack::IsEmpty(void)
{
	return this->m_stack.empty();
}

AEMatrix4f4& AEMatrixStack::GetMatrix(void)
{
	return (AEMatrix4f4&)(*this);
}

void AEMatrixStack::SetMatrix(const AEMatrix4f4& val)
{
	this->operator=(val);
}

AEMatrixStack::~AEMatrixStack(void)
{
	this->Clear();
}

