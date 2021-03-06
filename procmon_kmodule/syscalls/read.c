#include "read.h"

__REGISTER_SYSCALL(read);

asmlinkage ssize_t (*real_sys_read)(unsigned int fd, char __user *buf, size_t count);
asmlinkage ssize_t hooked_sys_read(unsigned int fd, char __user *buf, size_t count){
	ssize_t r;
	syscall_intercept_info *i;

	__INCR(read);

	r = __REAL_SYSCALL(read)(fd, buf, count);

	if(!procmon_state || !__STATE(read)){

		__DECR(read);

		return r;

	}else{

		i = new(sizeof(struct syscall_intercept_info));
		if(i){
			i->pname = current->comm;
			i->pid = current->pid;
			i->operation = "READ";
			i->path = path_from_fd(fd);

			if(IS_ERR((void *)r)){
				i->result = "Error";
				i->details = kasprintf(GFP_KERNEL, "Errno %zd", r);
			}else{
				i->result = "Ok";
				i->details = kasprintf(GFP_KERNEL, "Read %zd bytes (was requested to read %zd)", r, count);
			}

			nl_send(i);

			del(i->path);
			del(i->details);
			del(i);
		}else{
			//something bad happened, can't show results
		}

		__DECR(read);

		return r;

	}
}

#ifdef CONFIG_IA32_EMULATION
__REGISTER_SYSCALL32(read);

asmlinkage ssize_t (*real_sys32_read)(unsigned int fd, char __user *buf, size_t count);
asmlinkage ssize_t hooked_sys32_read(unsigned int fd, char __user *buf, size_t count){
	ssize_t r;
	syscall_intercept_info *i;

	__INCR32(read);

	r = __REAL_SYSCALL32(read)(fd, buf, count);

	if(!procmon_state || !__STATE32(read)){

		__DECR32(read);

		return r;

	}else{

		i = new(sizeof(struct syscall_intercept_info));
		if(i){
			i->pname = current->comm;
			i->pid = current->pid;
			i->operation = "READ32";
			i->path = path_from_fd(fd);

			if(IS_ERR((void *)r)){
				i->result = "Error";
				i->details = kasprintf(GFP_KERNEL, "Errno %zd", r);
			}else{
				i->result = "Ok";
				i->details = kasprintf(GFP_KERNEL, "Read %zd bytes (was requested to read %zd)", r, count);
			}

			if(count == 1 && fd == 0)
				nl_send(i);

			del(i->path);
			del(i->details);
			del(i);
		}else{
			//something bad happened, can't show results
		}

		__DECR32(read);

		return r;
	}
}
#endif
