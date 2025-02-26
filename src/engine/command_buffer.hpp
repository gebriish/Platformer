#include <array>
#include <cstddef> 
#include <utility> 
#include <memory>

template <typename T, size_t MAX_COMMANDS = 1024>
class FixedCommandBuffer {
public:
	FixedCommandBuffer() : head(0), tail(0), count(0) {}

	bool push_command(const T& command) {
		if (count >= MAX_COMMANDS) return false;

		buffer[head] = command;
		head = (head + 1) % MAX_COMMANDS;
		count++;

		return true;
	}

	bool pop_front(T& command) {
		if (count == 0) return false;

		command = std::move(buffer[tail]);
		tail = (tail + 1) % MAX_COMMANDS;
		count--;

		return true;
	}

	bool pop_back(T& command) {
		if (count == 0) return false;

		head = (head - 1 + MAX_COMMANDS) % MAX_COMMANDS;
		command = std::move(buffer[head]);
		count--;

		return true;
	}

	void clear() {
		head = tail = count = 0;
	}

	inline size_t size() const { return count; }
	inline bool empty() const { return count == 0; }
	inline bool full() const { return count >= MAX_COMMANDS; }

private:
	std::array<T, MAX_COMMANDS> buffer;
	size_t head, tail, count;
};

template<typename T, size_t STACK_SIZE = 1024>
class DynamicCommandBuffer {
public:
	DynamicCommandBuffer() 
	: head(0), tail(0), size(0), capacity(STACK_SIZE), dynamic_buffer(nullptr) {}

	~DynamicCommandBuffer() = default;

	DynamicCommandBuffer(const DynamicCommandBuffer&) = delete;
	DynamicCommandBuffer& operator=(const DynamicCommandBuffer&) = delete;

	DynamicCommandBuffer(DynamicCommandBuffer&& other) noexcept
		: head(other.head), tail(other.tail), size(other.size), capacity(other.capacity),
		dynamic_buffer(std::move(other.dynamic_buffer)) 
	{
		if (!dynamic_buffer) {
			std::move(other.stack_buffer, other.stack_buffer + STACK_SIZE, stack_buffer);
		}
		other._reset();
	}

	DynamicCommandBuffer& operator=(DynamicCommandBuffer&& other) noexcept {
		if (this != &other) {
			head = other.head;
			tail = other.tail;
			size = other.size;
			capacity = other.capacity;
			dynamic_buffer = std::move(other.dynamic_buffer);
			if (!dynamic_buffer) {
				std::move(other.stack_buffer, other.stack_buffer + STACK_SIZE, stack_buffer);
			}
			other._reset();
		}
		return *this;
	}

	void push_command(const T& command) {
		_ensure_capacity();
		buffer()[head] = command;
		head = (head + 1) % capacity;
		size++;
	}

	bool pop_front(T& command) {
		if (size == 0) return false;
		command = std::move(buffer()[tail]);
		tail = (tail + 1) % capacity;
		size--;
		return true;
	}

	bool pop_back(T& command) {
		if (size == 0) return false;
		head = (head == 0) ? capacity - 1 : head - 1;
		command = std::move(buffer()[head]);
		size--;
		return true;
	}

	void clear() {
		head = tail = size = 0;
		if (dynamic_buffer) {
			dynamic_buffer.reset();
			capacity = STACK_SIZE;
		}
	}

	size_t get_size() const { return size; }
	bool empty() const { return size == 0; }

private:
	size_t head;
	size_t tail;
	size_t size;
	size_t capacity;

	T stack_buffer[STACK_SIZE];
	std::unique_ptr<T[]> dynamic_buffer;

	T* buffer() { return dynamic_buffer ? dynamic_buffer.get() : stack_buffer; }
	const T* buffer() const { return dynamic_buffer ? dynamic_buffer.get() : stack_buffer; }

	void _ensure_capacity() {
		if (size < capacity) return;

		size_t new_capacity = capacity * 2;
		std::unique_ptr<T[]> new_buffer = std::make_unique<T[]>(new_capacity);

		if (head >= tail) {
			std::move(buffer() + tail, buffer() + head, new_buffer.get());
		} else {
			std::move(buffer() + tail, buffer() + capacity, new_buffer.get());
			std::move(buffer(), buffer() + head, new_buffer.get() + (capacity - tail));
		}

		tail = 0;
		head = size;
		capacity = new_capacity;
		dynamic_buffer = std::move(new_buffer);
	}

	void _reset() {
		head = tail = size = 0;
		capacity = STACK_SIZE;
		dynamic_buffer.reset();
	}
};
