#include <string>
#include <vector>
#include <memory>
#include <stdexcept>


namespace aengine
{
	class AERenderNode
	{
	public:
		std::string name;
		bool hidden;

		std::vector<std::shared_ptr<AERenderNodeInput> > inputs;
		std::vector<std::shared_ptr<AERenderNodeOutput> > ouputs;
	};

	class AERenderNodeVar
	{
		friend void Bind(std::shared_ptr<AERenderNodeOutput> ouput,shared_ptr<AERenderNodeInput> input);
	public:
		std::string name;
		AEColor color;
	};

	class _AERenderNodeOutput: std::enable_shared_from_this<_AERenderNodeOutput>
	{
	private:
		shared_ptr<_AERenderNodeOutput> GetPtr(void)
		{
			return shared_from_this();
		}

	public:
		template<typename T>
		&T GetData(void)
		{
			return static_cast<AERenderNodeOutput<T> >(this)->data;
		}
	};

	class _AERenderNodeInput
	{
	public:
		template<typename T>
		&T GetData(void)
		{
			return static_cast<AERenderNodeOutput<T> >(this)->GetData();
		}
	};

	template<typename T>
	class AERenderNodeOutput:
			public AERenderNodeVar,
			public _AERenderNodeOutput
	{
	public:
		typename T data;
	};

	template<typename T>
	class AERenderNodeInput:
			public AERenderNodeVar,
			public _AERenderNodeInput
	{
	private:
		std::weak_ptr<RenderNodeOutput<T> > source;
	public:
		bool IsBound(void)
		{
			source.expired();
		}

		const typename &T GetData(void)
		{
			if(IsBound())
				return source->GetData();
		}
	};

	template<typename T>
	void Bind(std::shared_ptr<AERenderNodeOutput<T> > ouput,shared_ptr<AERenderNodeInput<T> > input)
	{
		input.source.reset(std::static_pointer_cast<AERenderNodeOutput<T> >(output.GetPtr());
	}
}