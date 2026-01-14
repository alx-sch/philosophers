# philosophers

<p align="center">
    <img src=".assets/philosophers_badge.png" alt="philosophers_badge.png" />
</p>

## Fancy Printing

Add some flair to the philosopher's dinner by enabling additional visualization and simulation details.   
Compile with `make FANCY=1` to activate this feature.

<img src=".assets/fancy_printing.png" alt="fancy_printing.png" width="450" />

## Synchronization and Thinking Time

One key aspect of synchronization in this project is managing the thinking time of each philosopher. By subtracting the eating and sleeping times from the maximum time before death, the philosophers can think as long as possible without starving, ensuring fairness and reducing starvation risks for others:

```C
t_think = t_die - t_eat - t_sleep
```

This represents the theoretical maximum thinking time a philosopher can sustain without risking starvation. However, due to inherent delays, this value needs adjustment. By multiplying it with an`altruism_factor` in the range [0, 1], the thinking time can be scaled to balance individual and collective needs

- **Greedy Behavior (`altruism_factor = 0`):** Philosophers immediately attempt to eat after sleeping. This maximizes individual survival but risks starvation for others, especially with an odd number of philosophers.

- **Altruistic Behavior (`altruism_factor = 1`):** Philosophers wait the full thinking time before eating, prioritizing others but risking their own starvation due to delays.

- **Balanced Behavior (e.g., `altruism_factor = 0.9`):** Philosophers delay eating just enough to minimize starvation risks for others while maintaining their own needs. This strikes a balance between self-interest and collective fairness, ensuring that all philosophers are kept alive as long as possible.

Taken this into consideration, the thinking time is defined as:

```C
float altruism_factor = 0.9;

t_think = (t_die - t_eat - t_sleep) * altruism_factor;
```
---

## Surviving the Dinner

When the synchronization is set up correctly, the following rules ensure the survival of philosophers during the simulation:

- **Even number** of philosophers:    
  Philosophers can be grouped into two synchronized sets (even and odd positions). All philosophers survive if:
  - `t_eat < t_die / 2`, and
  - `t_die > (t_eat + t_sleep)`    
    A small buffer (e.g., 10 ms) should be added to `t_die` to account for potential system delays. Examples:
  - `./philo 4 610 300 300` → All philosophers survive.
  - `./philo 4 610 310 290`→ At least one philosopher starves.
 
- **Odd number** of philosophers:    
  Philosophers are divided into three groups (two synchronized sets and one unsynchronized philosopher). All philosophers survive if:
  - `t_eat < t_die / 3`, and
  - `(t_die / 2) > (t_eat + t_sleep)`    
    Again, adding a small buffer (e.g., 10 ms) to `t_die` is recommended. Examples:
  - `./philo 3 610 200 100` → All philosophers survive.
  - `./philo 3 610 210 90`→ At least one philosopher starves.
 
---

## Comparing `usleep` with a Custom Wait Function
I have implemented a custom wait function to address common issues with `usleep`, such as inconsistent delays. To illustrate the performance differences, I have included a test script: [`test_usleep.c`](https://github.com/alx-sch/42_philosophers/blob/main/test_usleep.c).

You can use this script to compare the performance of `usleep()` and the custom wait function on your system. The script measures the accuracy and consistency of both functions over multiple runs.

To compile and run the test, use the following command:
```bash
cc test_usleep.c -lm -o test_usleep
./test_usleep <sleep_duration_us> <sleep_intervals> <iterations>
./test_usleep 300000 100000 100
```

<img src=".assets/usleep_test_run.png" alt="usleep_test_run.png" width="400" />

### Why a Custom Wait Function?
In real-time simulations, precise timing is crucial, as even small inaccuracies can lead to undesired behavior, especially when multiple threads interact and delays accumulate. The standard `usleep()` function may introduce unexpected delays because of how the operating system handles process scheduling, resulting in less precise timing.

By actively checking the current time and using short sleep intervals, a custom wait function can mitigate these timing issues, resulting in more consistent and reliable behavior. This approach is especially important in scenarios like the Dining Philosophers problem, where precise timing and synchronization between threads are essential to avoid deadlocks and ensure proper execution.

```C
/**
Halts execution for a specified duration with improved precision.

 @param	duration_to_wait 	Duration in milliseconds for which to wait.
 @param	sim 			Pointer to a data struct to be freed
				in case of error.

 @return			`0` if the wait was successful;
				`1` if there was an error retrieving the time.
*/
int	precise_wait(int duration_to_wait)
{
	unsigned long long	time_stop_waiting;
	unsigned long long	current_time;

	time_stop_waiting = get_time();

	if (time_stop_waiting == 0)
		return (1);

	time_stop_waiting += duration_to_wait;

	while (1)
	{
		current_time = get_time();
		if (current_time == 0)
			return (1);
		if (current_time >= time_stop_waiting)
			break ;
		(void)usleep((duration_to_wait * 1000) / SLEEP_INTERVALS);
	}

	return (0);
}
```

### How `precise_wait` Works
The `precise_wait` function uses a combination of high-resolution time checks and short sleep intervals to provide a more accurate waiting period:

- **Time Measurement**: It starts by determining the current time using  `get_time()`, then calculates the target time ( `time_stop_waiting`) when the waiting period should end.

- **Short Sleep Intervals**: Instead of relying on a single long  `usleep()` call, which may be affected by system scheduling,  `precise_wait` uses short sleep intervals. This approach reduces the chance of oversleeping because it frequently checks the current time.   

- **Continuous Checking**: The function continuously checks if the current time has reached or surpassed  `time_stop_waiting`. This ensures that the wait duration is as precise as possible. The  `SLEEP_INTERVALS` constant controls how frequently the function checks the current time, striking a balance between accuracy and CPU usage.

## Acknowledgements  
- Simulation visualizations were generated using the [Philosophers visualizer](https://nafuka11.github.io/philosophers-visualizer-v2/) by nafuka11.   
- The project badge is from [this repository](https://github.com/ayogun/42-project-badges) by Ali Ogun.
