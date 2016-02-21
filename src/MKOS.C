/******************************************************************************
 File: mkos.c

 Date: August 1994

 (C) Williams Entertainment

 Mortal Kombat III Operating System Routines
******************************************************************************/
#include "system.h"

#ifdef SONY_PSX
#include "mksony.h"
#endif /* SONY_PSX */

#include "mkbkgd.h"
#include "mkos.h"
#include "mkgame.h"
#include "mkutil.h"
#include "mkmain.h"
#include "mkani.h"

/******************************************************************************
 Function: process_initialize()

 By: David Schwartz

 Date: Aug 5, 1994

 Parameters: None

 Returns: None

 Description:	This function sets up the active and free process link lists.
 				It also sets the active and process head ptrs.
******************************************************************************/
void process_initialize()
{
	int loop;

	active_head=NULL;
	free_head=process_ram;

#if MK_EJBBUG
	pcount=0;
#endif

	for (loop=0;loop<NUM_PROCESS-1;loop++)
		process_ram[loop].plink=&process_ram[loop+1];

	process_ram[NUM_PROCESS-1].plink=NULL;

	current_proc=(PROCESS *)&active_head;

	context_init_main((CONTEXT *)&main_context);

	return;
}

/******************************************************************************
 Function: process_create(WORD procid,void(*func_ptr)())

 By: David Schwartz

 Date: Aug 5, 1994

 Parameters: func_ptr - ptr to code to execute for process
			 procid - id of process

 Returns: ptr to newly created process

 Description:	This function removes a process from the free list and links it
				into the active list.  If there are no available processes then
				the function returns with no new process created.
******************************************************************************/
PROCESS *process_create(WORD procid,void (*func)(void))
{
	PROCESS *new_proc=NULL;

#if DEBUG
	short	test;

	if (procid==0)
	{
		test=0;
		while (test==0)
			POLLING;
	}

#endif

	if (free_head != NULL)
	{
		new_proc=free_head;			/* remove a process from free list */
		free_head=free_head->plink;		/* link free head to next free process */

		new_proc->plink=current_proc->plink;		/* link new process next to current process next */

		current_proc->plink=new_proc;		/* link current process to new process */

		new_proc->ptime=1;			/* set sleep time to start process right away */
		new_proc->procid=procid;		/* set id of newly created process */

		new_proc->p_context.c_frameptr=new_proc->p_context.c_stackptr=(ADDRESS)&(new_proc->p_context.c_stack[STKSIZE-3]);	/* set start of stack for process
																					   keeping in mind the non-leaf stack
																					   requirements for the R3000, 24 bytes  */

		new_proc->p_context.c_wakeup=(ADDRESS)func;		/* set wakeup point */
		new_proc->p_context.c_globalptr=main_context.c_globalptr;	/* set global ptr */
		new_proc->joyindex=0;							/* clear joy index */
		new_proc->p_comp_flag=PC_CLEAR;
		new_proc->pa8=current_proc->pa8;
		new_proc->pa9=current_proc->pa9;
		new_proc->a10=current_proc->a10;
		new_proc->a11=current_proc->a11;

#if MK_EJBBUG
		pcount++;
		if (pcount>pmax)
			pmax=pcount;
#endif

		return(new_proc);
	}

	ermac(16);
	return(NULL);
}

/******************************************************************************
 Function: process_kill(PROCESS *kill_proc)

 By: David Schwartz

 Date: Aug 5, 1994

 Parameters: kill_proc - ptr to process to remove from the active list and add
						 the process to the start of the free list

 Returns: None

 Description:	 This routine will remove the kill_proc from the active list and
				 place it on the free list.  This routine cannot kill the currently
				 active process, use suicide.  If you try to kill yourself, the
				 routine will just return
******************************************************************************/
void process_kill(PROCESS *kill_proc)
{
	PROCESS *proc_ptr;

	if (kill_proc==NULL || kill_proc==current_proc)			/* don't kill self, use suicide */
		return;

	proc_ptr=(PROCESS *)&active_head;

	while (proc_ptr->plink != NULL)
	{

		if (kill_proc==proc_ptr->plink)
		{
			proc_ptr->plink=kill_proc->plink;		/* unlink kill node from active */

			kill_proc->plink=free_head;		/* insert killed node at the top of the free list */
			free_head=kill_proc;

#if MK_EJBBUG
			pcount--;
#endif
			return;
		}

		proc_ptr=proc_ptr->plink;			/* next process */
	}

#if MK_EJBBUG
	wait_forever();
#endif /* MK_EJBBUG */

	return;
}

/******************************************************************************
 Function: process_kill_class(WORD killid,WORD mask)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters:	killid - class of processes to kill
				mask - mask bits of zero are don't cares

 Returns: None

 Description:	 This routine will kill a class of process from the active list
				 and place them on the free list.  This routine will not kill
				 the currently active process.
******************************************************************************/
void process_kill_class(WORD killid,WORD mask)
{
	PROCESS *proc_ptr;
	PROCESS *proc_prev;

	killid &=mask;							/* form match */

	proc_ptr=(PROCESS *)&active_head;

	while (proc_ptr->plink != NULL)
	{
		proc_prev=proc_ptr;					/* save previous process */

		proc_ptr=proc_ptr->plink;			/* point to next process */

		if (killid == (proc_ptr->procid & mask) && (proc_ptr != current_proc))
		{
			proc_prev->plink=proc_ptr->plink;	/* unlink killed node from active */

			proc_ptr->plink=free_head;		/* insert killed node at the top of the free list */

			free_head=proc_ptr;

			proc_ptr=proc_prev;				/* go back to prev */

#if MK_EJBBUG
			pcount--;
#endif
		}
	}

	return;
}

/******************************************************************************
 Function: PROCESS *process_exist(WORD existid,WORD mask)

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters:	existid - class of proc id to check
				mask - mask bits of zero are don't cares, mask to check

 Returns: pointer to process or NULL

 Description:	 This routine will find one calling process, other than the current process
******************************************************************************/
PROCESS *process_exist(WORD existid,WORD mask)
{
	PROCESS *proc_ptr;

	existid &=mask;							/* form match */

	proc_ptr=(PROCESS *)&active_head;

	while (proc_ptr->plink != NULL)
	{
		proc_ptr=proc_ptr->plink;

		if (existid == (proc_ptr->procid & mask) && (proc_ptr != current_proc))
			return(proc_ptr);
	}

	return(NULL);
}

/******************************************************************************
 Function: process_suicide(void)

 By: David Schwartz

 Date: Aug 5, 1994

 Parameters: None

 Returns: None

 Description:	 This routine will kill the current process and set the current
				 process to the previous process in the active list
******************************************************************************/
void process_suicide()
{
	PROCESS *proc_ptr,*proc_prev;
	PROCESS *proc_kill;

	proc_ptr=(PROCESS *)&active_head;

	proc_kill = current_proc;

	while (proc_ptr->plink != NULL)
	{
		proc_prev=proc_ptr;
		proc_ptr=proc_ptr->plink;

		if (proc_ptr==proc_kill)
		{
			proc_prev->plink=proc_kill->plink;		/* unlink killed node from active */

			proc_kill->plink=free_head;		/* insert killed node at the top of the free list */
			free_head=proc_kill;

			current_proc=proc_prev;

#if MK_EJBBUG
			pcount--;
#endif

			context_kill_switch((CONTEXT *)&main_context);

		}
	}

	wait_forever();

	return;
}

/******************************************************************************
 Function: process_dispatch(void)

 By: David Schwartz

 Date: Aug 5, 1994

 Parameters: None

 Returns: None

 Description:	 This routine is the main loop for the MK Operating System.  It
 				 will traverse the link list of active process and perform a context
				 switch on each active process.  The process will then run until completion
				 and a process_Suicide is invoked or until process_Sleep is invoked.  In either
				 case a context switch will bring the program flow back to this routine, which
				 will then move to the next process to invoke.
******************************************************************************/
#if 0
void validate_process(void)
{
	PROCESS *ptemp;

	ptemp=(PROCESS *)&active_head;

	while ( ptemp->plink != NULL )
	{
		if ( ptemp->plink<process_ram || ptemp->plink>(process_ram+sizeof(process_ram)) )
			LOCKUP;
		else ptemp=ptemp->plink;
	}
}
#endif

void process_dispatch()
{
	current_proc=(PROCESS *)&active_head;				/* set current to start of list */

	POLLING;

	timer_interrupt();

	irq_timer=0;


	if (f_pause<PAUSE_OFF)
	{
		/* normal dispatch routine */
		while (current_proc->plink != NULL)
		{
			current_proc=current_proc->plink;

			if (--(current_proc->ptime)==0)		/* sleep until process time is zero */
			{
				context_switch(&(current_proc->p_context),(CONTEXT *)&main_context);
//				validate_process();
			}
		}
	}
	else
	{
		/* pause dispatch, only look at pause class processes */
		while (current_proc->plink != NULL)
		{
			current_proc=current_proc->plink;
			if ((current_proc->procid & 0xff00)== PID_PAUSE)		// only check paused processes
			{
				if (--(current_proc->ptime)==0)		/* sleep until process time is zero */
					context_switch(&(current_proc->p_context),(CONTEXT *)&main_context);
			}
		}
	}

	return;

}

/******************************************************************************
 Function: process_sleep(WORD sleep_time)

 By: David Schwartz

 Date: Aug 5, 1994

 Parameters: None

 Returns: None

 Description:	 This routine will set a process's sleep time and then context switch
				 back to the dispatch routine.
******************************************************************************/
void process_sleep(WORD sleep_time)
{
#if PAL_VERSION==5
	current_proc->ptime=(sleep_time*50)/60;			/* set sleep time */
	if ( current_proc->ptime==0 )
		current_proc->ptime=1;
#else
	current_proc->ptime=sleep_time;			/* set sleep time */
#endif

	context_switch((CONTEXT *)&main_context,&(current_proc->p_context));

	return;
}

/******************************************************************************
 Function: timer_interrupt()

 By: David Schwartz

 Date: Aug 8, 1994

 Parameters: None

 Returns: None

 Description:	This function runs once every time through the exec loop (MAIN DISPATCHER)
			    It is responsible for updating a timer.  This routine can be used for auditing
				and other timeing issues if needed.
******************************************************************************/
void timer_interrupt(void)
{
	timecnt +=irq_timer;

	if (timecnt>ONE_SECOND)
	{
		timecnt=0;
	}

	return;
}


