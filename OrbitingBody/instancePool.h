#pragma once

template <typename T>
class instancePool
{
public:
	unsigned count;
	T *d;

	instancePool()
	{
		size = 4;
		d = (T*)malloc(size * sizeof(T));
		count = 0;
	}

	instancePool(unsigned size)
	{
		d = (T*)malloc(size * sizeof(T));
		count = 0;
	}

	~instancePool()
	{
		free(d);
	}

	void addInstance(T *id)
	{
		count++;
		if (count == size)
			reallocPool();
		std::uninitialized_copy(id, id + 1, &d[count - 1]);
	}

	void removeInstance(unsigned i)
	{
		d[i] = d[count];
		count--;
	}

private:
	unsigned size;

	void reallocPool()
	{
		size *= 2;
		d = (T*)realloc(d, size * sizeof(T));
	}
};