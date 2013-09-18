
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(conversion_sync, unsigned long, address, int, flags)
{
  struct vm_area_struct *vma;

  //down_read(&current->mm->mmap_sem);
  vma = find_vma(current->mm, address);
  //for ksnap, just do get/make and return                                                                                            
  if (vma && mmap_snapshot_instance.is_snapshot &&
      mmap_snapshot_instance.is_snapshot(vma, NULL, NULL) &&
            mmap_snapshot_instance.snapshot_msync &&
      ((flags &  MS_KSNAP_GET) || (flags & MS_KSNAP_MAKE) ||
       (flags & MS_KSNAP_GET_MERGE))){    //TODO: for commit, need to relax these constraints                                         
    mmap_snapshot_instance.snapshot_msync(vma, flags);                    //TODO: this function name in the struct should change      
  }

  //up_read(&current->mm->mmap_sem);
}
