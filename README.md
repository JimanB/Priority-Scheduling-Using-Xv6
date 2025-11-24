# xv6 Priority Scheduling Implementation

## Overview
This project implements priority-based scheduling in the xv6 operating system, replacing the default round-robin scheduler. Processes with higher priority (lower numerical values) are scheduled earlier, with round-robin scheduling applied to processes of equal priority.

## Features
- **Priority-Based Scheduling**: Processes are scheduled based on priority levels
- **Dynamic Priority Adjustment**: System call to modify process priorities at runtime
- **Enhanced Process Display**: Updated `ps` command shows priority values
- **Backward Compatibility**: Maintains existing xv6 functionality
- **Preemptive Scheduling**: Higher priority processes can preempt lower priority ones

## Priority System

### Priority Range
- **Minimum Priority**: 0 (highest priority)
- **Maximum Priority**: 20 (lowest priority) 
- **Default Priority**: 10 (for new processes)

### Priority Values
- **Lower numbers = Higher priority** (0 is highest, 20 is lowest)
- **PRIO_MIN**: 0 (highest priority)
- **PRIO_MAX**: 20 (lowest priority) 
- **PRIO_DEF**: 10 (default priority for new processes)

## System Calls

### 1. `ps` - Process State Display
Displays comprehensive process information including priorities.

**Usage**: `ps`

**Sample Output**:
```
pid     name            state           priority
--------------------------------------------------
1       init            runnable        10
2       sh              sleeping        10
3       pi              running         5
```

### 2. `set` - Set Process Priority
Dynamically modifies process priority.

**Usage**: `set <pid> <priority>`

**Parameters**:
- `pid`: Process ID to modify
- `priority`: New priority value (0-20)

**Return**: 0 on success, -1 on error

## Implementation Details

### Modified Files

#### Kernel Level
- **`kernel/param.h`**: Added priority constants (`PRIO_MIN`, `PRIO_MAX`, `PRIO_DEF`)
- **`kernel/proc.h`**: Added `prio` field to process structure
- **`kernel/proc.c`**: 
  - Modified `allocproc()` to set default priority
  - Implemented priority-based scheduler
  - Added `setpriority()` function
  - Enhanced `procdump()` to show priorities
- **`kernel/syscall.c`**: Registered new system calls
- **`kernel/syscall.h`**: Added system call numbers (`SYS_ps`, `SYS_set`)
- **`kernel/sysproc.c`**: Implemented `sys_ps()` and `sys_set()` handlers
- **`kernel/defs.h`**: Added `setpriority()` function prototype

#### User Level
- **`user/user.h`**: Added user-space system call declarations
- **`user/usys.pl`**: Added system call stubs
- **`user/ps.c`**: User program for `ps` command
- **`user/set.c`**: User program for `set` command
- **`user/pi.c`**: Enhanced CPU-intensive test program

### Scheduler Algorithm
The new scheduler implements a **priority-based scheduling with round-robin**:

1. **Priority Scanning**: Scans from highest priority (0) to lowest (20)
2. **Process Selection**: For each priority level, selects processes in round-robin fashion
3. **Preemption**: Higher priority processes can preempt lower priority ones when they become runnable
4. **Fairness**: Processes at same priority level share CPU time equally

```c
// Simplified scheduler logic
for(current_prio = PRIO_MIN; current_prio <= PRIO_MAX; current_prio++) {
    for(p = proc; p < &proc[NPROC]; p++) {
        if(p->state == RUNNABLE && p->prio == current_prio) {
            // Run this process
            // Implicit round-robin within same priority
        }
    }
}
```

## Installation & Usage

### 1. Add Files to xv6
Copy all modified and new files to your xv6 source directory and update the Makefile:

```makefile
# In UPROGS add:
_ps\
_set\
_pi\
```

### 2. Compile and Run
```bash
make
make qemu
```

### 3. Testing the Implementation

#### Basic Priority Scheduling Test
```bash
# Start multiple test processes
pi &
pi &
pi &

# Check current process states and priorities
ps

# Modify priorities to see scheduling changes
set 4 5    # Set PID 4 to high priority
set 5 15   # Set PID 5 to low priority
ps
```

#### Comprehensive Test Scenario
```bash
# Terminal 1: Start high-priority process
set 3 0    # Give shell highest priority

# Terminal 2: Start test processes with different priorities
pi &       # Default priority (10)
set $! 5   # Set last background PID to medium priority

pi &       # Default priority (10)
set $! 15  # Set last background PID to low priority

# Monitor scheduling behavior
ps
```

## Testing Strategy

### 1. Basic Functionality
- Verify `ps` command displays priorities
- Test `set` command with valid and invalid parameters
- Confirm default priority assignment

### 2. Priority Scheduling Behavior
- Create multiple processes with different priorities
- Observe that higher priority processes get more CPU time
- Verify round-robin behavior within same priority level

### 3. Dynamic Priority Changes
- Modify priorities of running processes
- Observe immediate scheduling changes
- Test edge cases (minimum and maximum priorities)

### 4. Preemption Testing
- Start low-priority CPU-intensive process
- Start high-priority process
- Verify high-priority process preempts the low-priority one

## Sample Test Session

```bash
$ pi &
$ pi &
$ ps
pid     name            state           priority
--------------------------------------------------
1       init            runnable        10
2       sh              sleeping        10
3       pi              running         10
4       pi              runnable        10

$ set 3 5
$ set 4 15
$ ps
pid     name            state           priority
--------------------------------------------------
1       init            runnable        10
2       sh              sleeping        10
3       pi              running         5
4       pi              runnable        15
```

## Expected Behavior

1. **High Priority Advantage**: Process with priority 5 should get significantly more CPU time than process with priority 15
2. **Same Priority Fairness**: Processes with same priority should share CPU time equally
3. **Dynamic Adjustments**: Priority changes should take effect immediately
4. **System Stability**: No crashes or unexpected behavior with priority changes

## Limitations

- **Priority Inversion**: No explicit handling of priority inversion scenarios
- **No Priority Inheritance**: Simple priority assignment without inheritance mechanisms
- **Static Time Quantum**: Fixed time slices for all priority levels
- **No Load Balancing**: Basic scheduling without CPU load consideration

## Performance Considerations

- **Efficient Scanning**: Priority levels are scanned from high to low for optimal performance
- **Minimal Overhead**: Priority checks add negligible overhead to scheduler
- **Scalable Design**: Works efficiently with xv6's maximum process limit (NPROC)

## Files Included

### Modified Kernel Files
- `kernel/param.h` - Priority constants
- `kernel/proc.h` - Process structure modification
- `kernel/proc.c` - Scheduler and priority implementation
- `kernel/syscall.c` - System call registration
- `kernel/syscall.h` - System call numbers
- `kernel/sysproc.c` - System call handlers
- `kernel/defs.h` - Function prototypes

### New User Programs
- `user/ps.c` - Process status display
- `user/set.c` - Priority setting utility
- `user/pi.c` - Enhanced test program

### Modified User Files
- `user/user.h` - System call declarations
- `user/usys.pl` - System call stubs

## Notes

- The implementation maintains full backward compatibility
- No unnecessary modifications to existing xv6 functionality
- Proper locking mechanisms ensure thread safety
- Clean separation between scheduling policy and mechanism
