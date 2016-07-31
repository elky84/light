
namespace light
{

template <typename SizeType>
void LinearBuffer<SizeType>::initialize()
{
	m_head = 0;
	m_tail = 0;
}

template <typename SizeType>
bool LinearBuffer<SizeType>::extend(SIZE len)
{
    if (len <= m_buffer_size)
        return false;

    char* ptr = BoostPool<char[UNIT]>::Instance()->allocate(len);
    if (ptr == NULL)
        return false;

    memcpy(ptr, m_buffer, m_tail);
    free();
    
    m_buffer = ptr;
    m_buffer_size = len;
    return true;
}

template <typename SizeType>
bool LinearBuffer<SizeType>::jump_head(Position position)
{
	if(get_tail() < get_head() + position)
	{
		LOG_ERROR(L"[%p] %d %d %d", this, get_head(), get_tail(), position);
		return false;
	}

	m_head += position;
	return true;
}

template <typename SizeType>
bool LinearBuffer<SizeType>::jump_tail(Position position)
{
	if(get_size() < get_tail() + position)
	{
		LOG_ERROR(L"[%p] %d %d %d", this, get_head(), get_tail(), position);
		return false;
	}

	m_tail += position;
	return true;
}

template <typename SizeType>
void LinearBuffer<SizeType>::compress()
{
	if(get_head() == get_tail()) //다 읽었다면
	{
		initialize();
	}
	else // 아직 남았다면
	{
		SIZE using_size = get_length();
		CopyMemory(get_buffer(), get_head_buffer(), using_size);
		initialize();
		m_tail += using_size;
	}
}

template <typename SizeType>
bool LinearBuffer<SizeType>::write(const SIZE len, const void* ptr, const SIZE len2, const void* ptr2)
{
    if (m_buffer == NULL)
    {
        Seh::RaiseException(__FUNCTION__, m_buffer_size, len);
        return false;
    }

	SIZE write_len = len + len2;
	if(get_size() < get_tail() + write_len)
	{
		LOG_ERROR(L"[%p] %d %d %d", this, get_size(), get_tail(), write_len);
		return false;
	}

	if(ptr)
	{
		CopyMemory(get_buffer(get_tail()), ptr, len);
	}

	if(ptr2)
	{
		CopyMemory(get_buffer(get_tail() + len), ptr2, len2);
	}

	m_tail += write_len;
	return true;
}

} //namespace light