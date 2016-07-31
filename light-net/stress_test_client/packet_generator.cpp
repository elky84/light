#include "stdafx.h"
#include "packet_generator.h"

packet_generator::packet_generator()
{

}

packet_generator::~packet_generator()
{

}


bool packet_generator::random(light::LinearNormalBuffer* buffer)
{
    light::net::PACKET_SIZE size = (light::net::PACKET_SIZE)((rand() % (std::numeric_limits<unsigned short>::max)()) + 1);

    light::net::PacketHeader header(light::net::PACKET_ID(rand() % 255), size + light::net::PacketHeader::header_size());
    buffer->write((unsigned short)header.header_size(), &header);

    for (unsigned int i = 0; i < size; i++)
    {
        char ch = (char)(rand() % 255);
        buffer->write(1, &ch);
    }
    return true;
}

bool packet_generator::light(light::LinearNormalBuffer* buffer)
{
    light::net::PACKET_SIZE size = rand() % 100;

    light::net::PacketHeader header(light::net::PACKET_ID(rand() % 255), size + light::net::PacketHeader::header_size());
    buffer->write((unsigned short)header.header_size(), &header);

    for (unsigned int i = 0; i < size; i++)
    {
        char ch = (char)(rand() % 255);
        buffer->write(1, &ch);
    }
    return true;
}

bool packet_generator::heavy(light::LinearNormalBuffer* buffer)
{
    light::net::PACKET_SIZE size = std::numeric_limits<unsigned short>::max() + rand() % std::numeric_limits<unsigned short>::max();

    light::net::PacketHeader header(light::net::PACKET_ID(rand() % 255), size + light::net::PacketHeader::header_size());
    buffer->write((unsigned short)header.header_size(), &header);

    for (unsigned int i = 0; i < size; i++)
    {
        char ch = (char)(rand() % 255);
        buffer->write(1, &ch);
    }
    return true;
}
