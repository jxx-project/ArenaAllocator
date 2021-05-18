#ifndef CHUNK_H_INCLUDED
#define CHUNK_H_INCLUDED

class Arena;

struct Chunk
{
	void* data;
	Arena* arena;
	bool allocated;
};

#endif // CHUNK_H_INCLUDED
