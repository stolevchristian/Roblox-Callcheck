# Roblox Callcheck (2021)
This project demonstrates **bypassing Roblox´s callcheck** using a smart trick with INT3´s. Once a CClosure has been pushed to Roblox´s stack or environment they will perform integrity checks on it.
The integrity checks are not that complicated and can be compared to the code below:
```c++
BOOL isMaliciousFunction(void* ptr)
{
  return RBX::IsInsideOurMemory(ptr);
}
```

When we push the CClosure from our DLL, it's mapped to our "non-whitelisted" memory and Roblox can detect that. Instead we push INT3´s and once they´re hit (Roblox trying to execute them) we can redirect execution to our real CClosure using the VEH.
Pretty smart idea, not invented by me but rewritten by me.

> This is way outdated, do not try it.
