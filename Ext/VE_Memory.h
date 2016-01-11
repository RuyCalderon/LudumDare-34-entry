#if !defined(VE_MEMORY_H)
#define VE_MEMORY_H

#define ALIGN(Amount,Alignment) (Amount - ((Amount)%Alignment) + Alignment) 

inline bool 
StringIdentical(char *First, char *Second)
{
	int i = 0;
	while(First[i] && Second[i])
	{
		if(First[i] != Second[i])
		{
			return false;
		}
		i++;
	}
	return true;
}

struct MemoryArena
{
	void *FirstAddress;
	void *CurrentAddress;
	size_t Allocated;
	size_t Used;
	size_t Available;
	
	MemoryArena *NextArena;
	MemoryArena *PreviousArena;

	const char *ArenaName;
};

struct AllocatedMemory
{
	void *Memory;
	size_t Allocated;
	size_t Used;
	size_t Available;

	void *NextAddress;
	void *FirstAddress;
	MemoryArena *CurrentArena;
};

MemoryArena *FindArena(AllocatedMemory *ArenaManager, const char* ArenaToFind)
{
	MemoryArena *ArenaToCheck = ArenaManager->CurrentArena;

	while(ArenaToCheck)
	{
		if(StringIdentical((char *)ArenaToCheck->ArenaName,(char *)ArenaToFind))
		{
			return ArenaToCheck;
		}
		ArenaToCheck = ArenaToCheck->PreviousArena;
	}
	return 0;
}

MemoryArena *PushMemoryArena(AllocatedMemory *GameMemory, size_t ArenaSize, const char* ArenaName)
{
	size_t TotalSizeOfArena = ALIGN(ArenaSize + sizeof(ArenaName),8);
	size_t AlignedArenaStruct = ALIGN(sizeof(MemoryArena) + sizeof(ArenaName),8);
	Assert(TotalSizeOfArena > AlignedArenaStruct);

	if(GameMemory->Available < TotalSizeOfArena)
	{
		return 0;
	}
	else
	{
		MemoryArena *NewArena = (MemoryArena *)GameMemory->NextAddress;
		NewArena->ArenaName = ArenaName;
		GameMemory->Used += TotalSizeOfArena;
		GameMemory->Available = GameMemory->Allocated - GameMemory->Used;
		GameMemory->NextAddress = (void*)(((char*)GameMemory->NextAddress) + TotalSizeOfArena);

		NewArena->FirstAddress = (void*)((char*)(NewArena) + AlignedArenaStruct);
		NewArena->CurrentAddress = NewArena->FirstAddress;
		NewArena->Available = TotalSizeOfArena - AlignedArenaStruct;
		NewArena->Allocated = NewArena->Available;
		NewArena->Used = 0;
			
		if(GameMemory->CurrentArena)
		{
			NewArena->PreviousArena = GameMemory->CurrentArena;
			NewArena->NextArena = 0;
			GameMemory->CurrentArena->NextArena = NewArena;
			GameMemory->CurrentArena = NewArena;
		}
		else
		{
			NewArena->PreviousArena = 0;
			NewArena->NextArena = 0;
			GameMemory->CurrentArena = NewArena;
		}

		return NewArena;
	}
}

void
PopMemoryArena(AllocatedMemory *GameMemory)
{
	size_t StructSize = ALIGN(sizeof(MemoryArena), 8);
	size_t SizeReserved = GameMemory->CurrentArena->Allocated + StructSize;

	GameMemory->CurrentArena = GameMemory->CurrentArena->PreviousArena;
	GameMemory->CurrentArena->NextArena = 0;
	
	GameMemory->Used -= SizeReserved;
	GameMemory->Available += SizeReserved;
	GameMemory->NextAddress = (void*)((char*)GameMemory->NextAddress - SizeReserved);
}

#define VE_PushArray(STRUCT, Count, Arena) (STRUCT *)VE_PrepAlloc((sizeof(STRUCT)*Count), Arena)
#define VE_PushAlloc(STRUCT, Arena) (STRUCT *)VE_PrepAlloc(sizeof(STRUCT), Arena);
void* 
VE_PrepAlloc(size_t MemoryAmount, MemoryArena *Arena)
{
	size_t AllocationSize = ALIGN(MemoryAmount,8);
	if(Arena->Available < AllocationSize)
	{
		return 0;
	}
	else
	{
		void *NewAddress = Arena->CurrentAddress;
		Arena->Used += AllocationSize;
		Arena->Available = Arena->Allocated - Arena->Used;
		Arena->CurrentAddress = (void*)((char *)Arena->CurrentAddress + AllocationSize);
		return NewAddress;
	}
}

#define VE_PopFree(STRUCT, Arena) VE_PrepFree((void *)STRUCT, sizeof(*STRUCT), Arena)
void
VE_PrepFree(void *Address, size_t MemUsed, MemoryArena *Arena)
{
	Assert((Arena->CurrentAddress == (void*)((char*)Address + MemUsed)));
	Arena->CurrentAddress = Address;
	Arena->Used-=MemUsed;
	Arena->Available = Arena->Allocated - Arena->Used;
} 
#endif
