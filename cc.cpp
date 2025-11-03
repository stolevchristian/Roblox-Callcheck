int MAX_INT3 = 200; /* If you're pushing over 200 functions change this to a higher number */
std::vector<int>       breakpoints; /* Store INT3's */
std::vector<uintptr_t> cclosures;   /* Store closures */

LONG WINAPI VEH(PEXCEPTION_POINTERS exception)
{
    switch (exception->ExceptionRecord->ExceptionCode)
    {
        case (DWORD)0x80000003L: /* INT3 Exception */
        {
            int int3idx = 0;
            for(int INT3 : breakpoints) /* Loop all int3's */
            {
                if (exception->ContextRecord->Eip == INT3) /* Is it the same int3 as we stired? */
                {
                    exception->ContextRecord->Eip = (DWORD)(cclosures[int3idx]); /* Get the specified closure for that Int3 */
                    return EXCEPTION_CONTINUE_EXECUTION; 
                }
                int3idx += 1;
            }
            return -1;
        }
        default: 
            return 0;
    }
    return 0;
}

DWORD MAP_INT3() {
    int int3_index = 0;
    DWORD BASE = rebase(0x400000); /* Replace with modulehandle ngl */
    const char i3_8opcode[8] = { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC };
    for (int i = 0; i < INT_MAX; i++) {
        if (memcmp((void*)(BASE + i), i3_8opcode, sizeof(i3_8opcode)) == 0) {  
            int3_index += 1;
            if (int3_index > MAX_INT3) /* Max int for storage */
            {
                break;
            }
            printf("[%d] Int3 Cached.\n", int3_index);
            breakpoints.push_back(BASE + i); /* Store Int3 */
        }
    }
    return NULL;
}
int ConvertClosure(uintptr_t cclosure = 0)
{
    cclosures.push_back(cclosure); /* Store closure at top */
    int closure_index = cclosures.size() - 1; /* Get closure position in vector */
    int assigned_int3 = breakpoints[closure_index]; /* Get assigned Int3 for closure */
    return assigned_int3; /* Return assigned Int3 */
}

int main()
{
    ConsoleBypass("PCallcheck");
    printf("Creating Map..\n");
    MAP_INT3();
    AddVectoredExceptionHandler(1, VEH);
  
    Roblox::Pushcclosure(ConvertClosure(WHATEVER_CFUNC), 1); 
}
