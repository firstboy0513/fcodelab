/* Copyright (C) 
* 2013 - firstboy0513
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/
/**
* @file elevator_best_park.c
* @brief elevator best park floor.
* @author firstboy0513
* @version 0.0.1
* @date 2013-12-07
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* @brief elevator best park floor use exhaustion to calculate it.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param pbest_floor_ladder output best floor ladder cost.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
*
* @return best floor park, or return -1 means error occur.
*/
int elevator_best_park_use_exhaustion(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder, int calorie) {
	if (!floor_person_number || floor_length <= 0) {
		printf("[ERR] -- elevator best park invalid input.\n");
		return -1;
	}
	int min_floor_ladder = -1, best_floor_park = 0;
	int i = 0, j = 0;
	for (i = 0; i < floor_length; i ++) { // assume park in the ith floor
		int current_floor_ladder = 0;
		for (j = 0; j < i; j ++) {
			current_floor_ladder += floor_person_number[j] * (i - j);
		}
		for (j = i + 1; j < floor_length; j ++) {
			current_floor_ladder += floor_person_number[j] * (j - i) * calorie;
		}
		if (-1 == min_floor_ladder 
			|| current_floor_ladder < min_floor_ladder) {
			min_floor_ladder = current_floor_ladder;
			best_floor_park = i;
		}
		printf("Now is park %dth floor, it cost %d floor ladder.\n", 
			i+1, current_floor_ladder);
	}
	if (min_floor_ladder >= 0 && pbest_floor_ladder) {
		*pbest_floor_ladder = min_floor_ladder;
	}
	return best_floor_park;
}

/**
* @brief elevator best park floor not use exhaustion.
* Assume now is calculate the xth floor, and the N1 means the bottom blow x 
* floor people number, and N2 means current xth floor people number, and N3 
* means the top in the head of x floor people number. And Y is the current xth
* floor will cost the floor ladders.
* So, if move to (x+1)th floor, then the floor ladder cost will be change to:
* 	Y + N1 + N2 - N3   ==>   Y + (N1 + N2 - N3)
* if think about calorie, then Y + (N1 + N2 - calorie*N3)
* and if move to (x-1)th floor, the the floor ladder cost will be change to:
* 	Y - N1 + N2 + N3   ==>   Y - (N1 - N2 - N3)
* if think about calorie, then Y - (N1 - calorie*N2 - calorie*N3)
* So, if (N1 + N2 - N3) < 0, it means (x+1)th floor is better than xth floor.
* And if (N1 - N2 - N3) > 0, it means (x-1)th fllor is better than xth floor.
* We know the beginning floor begin from 1, there maybe (N1 + N2 - N3) < 0, 
* so we just traversal from it, util it come to be (N1 + N2 - N3) >= 0, then 
* do not traversal, just stop it, we can get the best ladder floor.
* This method just cost O(N) times.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param pbest_floor_ladder output best floor ladder cost.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
*
* @return best floor park, or return -1 means error occur.
*/
int elevator_best_park_no_exhaustion(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder, int calorie) {
	if (!floor_person_number || floor_length <= 0) {
		printf("[ERR] -- elevator best park invalid input.\n");
		return -1;
	}
	int best_floor_park = 0;
	int N1 = 0, N2 = 0, N3 = 0, Y = 0;
	// set the N3 and beginning Y first
	int i = 0;
	for (N2 = floor_person_number[0], i = 1; i < floor_length; i ++) {
		N3 += floor_person_number[i];
		Y += floor_person_number[i] * (i - 0) * calorie;
	}
	for (i = 1; i < floor_length; i ++) {
		if ((N1 + N2) >= (calorie * N3)) {
			break;
		}
		best_floor_park = i;
		Y += ((N1 + N2) - (calorie * N3));
		N1 += N2;
		N2 = floor_person_number[i];
		N3 -= floor_person_number[i];
	}
	if (Y >= 0 && pbest_floor_ladder) {
		*pbest_floor_ladder = Y;
	}
	return best_floor_park;
}

/**
* @brief use median to calculate the elevator best park floor.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param pbest_floor_ladder output best floor ladder cost.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
*
* @return best floor park, or return -1 means error occur.
*/
int elevator_best_park_use_median(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder, int calorie) {
	if (!floor_person_number || floor_length <= 0) {
		printf("[ERR] -- elevator best park invalid input.\n");
		return -1;
	}
	int j = 0;
	int* dup_floor_person_number = (int*) malloc(sizeof(int)*floor_length);
	for (j = 0; j < floor_length; j ++) {
		dup_floor_person_number[j] = floor_person_number[j];
	}
	int left_index = 0; int right_index = floor_length - 1;
	dup_floor_person_number[right_index] *= calorie;
	while (right_index > left_index) {
		while (dup_floor_person_number[left_index] <= 0 
			&& right_index > left_index) { left_index ++; }
		dup_floor_person_number[left_index] --;
		while (dup_floor_person_number[right_index] <= 0
			&& right_index > left_index) { right_index --;
				dup_floor_person_number[right_index] *= calorie; }
		dup_floor_person_number[right_index] --;
	}
	// set best floor ladder cost
	if (pbest_floor_ladder) {
		*pbest_floor_ladder = 0;
		for (j = 0; j < left_index; j ++) {
			*pbest_floor_ladder += floor_person_number[j] * (left_index-j);
		}
		for (j = left_index + 1; j < floor_length; j ++) {
			*pbest_floor_ladder += 
				floor_person_number[j] * (j-left_index) * calorie;
		}
	}
	free(dup_floor_person_number);
	return left_index;
}

/**
* @brief use balance method to calculate it, like shoulder pole, the left 
* weight and the right weight are balance. It is a middle weight if has no 
* calorie, if has the calorie then mulitiplied by the calorie weight in the 
* right weight.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param pbest_floor_ladder output best floor ladder cost.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
*
* @return best floor park, or return -1 means error occur.
*/
int elevator_best_park_use_balance(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder, int calorie) {
	if (!floor_person_number || floor_length <= 0) {
		printf("[ERR] -- elevator best park invalid input.\n");
		return -1;
	}
	int best_floor_park = 0;
	int i = 0, j = 0;
	int total_person_number = 0;
	for (i = 0; i < floor_length; i ++) {
		total_person_number += floor_person_number[i];
	}
	int left_number = 0, right_number = total_person_number * calorie;
	for (i = 0; i < floor_length; i ++) {
		left_number += floor_person_number[i];
		right_number -= floor_person_number[i] * calorie;
		if (left_number >= right_number) {
			best_floor_park = i;
			break;
		}
	}
	// set best floor ladder cost
	if (pbest_floor_ladder) {
		*pbest_floor_ladder = 0;
		for (j = 0; j < best_floor_park; j ++) {
			*pbest_floor_ladder += floor_person_number[j] * (best_floor_park-j);
		}
		for (j = best_floor_park + 1; j < floor_length; j ++) {
			*pbest_floor_ladder += 
				floor_person_number[j] * (j-best_floor_park) * calorie;
		}
	}
	return best_floor_park;
}

/**
* @brief best result struct use to find the best parks.
*/
typedef struct _BEST_RESULT {
	int cost;  // best cost
	int floor; // best floor
} BEST_RESULT; 

/**
* @brief compute current best park in given park count and remain floors.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
* @param best_cost best cost array point.
* @param floor_num remain floor count.
* @param park_count park count.
*
* @return the best cost ladder.
*/
int compute_best_park(const int* floor_person_number, const int floor_length, 
	int calorie, BEST_RESULT** best_cost, int floor_num, int park_count) {
	if (best_cost[floor_num][park_count].cost > 0) {
		return best_cost[floor_num][park_count].cost;
	}
	int start_floor = floor_length - floor_num;
	if (start_floor >= floor_length) return 0;
	int i = 0, j = 0, current_cost = 0;
	if (park_count == 0) {
		if (start_floor == 0) return 0;
		for (j = start_floor; j < floor_length; j ++) {
			current_cost += floor_person_number[j] * (j - start_floor + 1);
		}
		return current_cost;
	}
	int left = start_floor, right = floor_length - park_count + 1;
	for (i = left; i < right; i ++) { // assume i is the park floor
		current_cost = 0;
		int middle = (start_floor + i) / 2;
		if (start_floor == 0) {
			for (j = start_floor; j < i; j ++) {
				current_cost += floor_person_number[j] * (i - j);
			}
		} else {
			for (j = start_floor; j <= middle; j ++) {
				current_cost += floor_person_number[j] * (j - start_floor + 1);
			}
			for (j = middle + 1; j < i; j ++) {
				current_cost += floor_person_number[j] * (i - j);
			}
		}
		if (park_count > 0) {
			current_cost += compute_best_park(floor_person_number, floor_length,
				calorie, best_cost, floor_length-i-1, park_count-1);
		}
		if (-1 == best_cost[floor_num][park_count].cost 
			|| current_cost < best_cost[floor_num][park_count].cost) {
			best_cost[floor_num][park_count].cost = current_cost;
			best_cost[floor_num][park_count].floor = i;
		}
	}
	return best_cost[floor_num][park_count].cost;
}

/**
* @brief calculate elevator best parks.
* It using dynamic programming to store result.
*
* @param floor_person_number the person number in which floor they want to go.
* @param floor_length floor_person_number array length.
* @param pbest_floor_ladder output best floor ladder cost.
* @param calorie the ladder get up stair to use calorie as more weight to go 
* down stair.
* @param park_count the count can be park.
* @param best_parks output best parks in a int array.
*
* @return return 0 is ok, or return -1 means error occur.
*/
int* elevator_best_parks(const int* floor_person_number, 
	const int floor_length, int* pbest_floor_ladder, int calorie, 
	int park_count) {
	if (!floor_person_number || floor_length <= 0) {
		printf("[ERR] -- elevator best park invalid input.\n");
		return NULL;
	}
	// initialize best result
	int i = 0, j = 0;
	BEST_RESULT** best_cost = (BEST_RESULT**) malloc(
		sizeof(BEST_RESULT*) * (floor_length+1));
	for (i = 0; i <=floor_length; i ++) {
		best_cost[i] = (BEST_RESULT*) 
			malloc(sizeof(BEST_RESULT) * (park_count+1));
		for (j = 0; j <= park_count; j ++) {
			best_cost[i][j].cost = best_cost[i][j].floor = -1;
		}
	}

	// compute the best park
	compute_best_park(floor_person_number, floor_length, calorie, best_cost, 
		floor_length, park_count);
	// debug output
	for (i = 1; i <= floor_length; i ++) {
		for (j = 1; j <= park_count; j ++) {
			printf("best_cost[%d][%d] = %d, floor = %d\n", i, j, 
				best_cost[i][j].cost, best_cost[i][j].floor);
		}
	}
	int* best_parks = (int*) malloc(sizeof(int) * park_count);
	memset(best_parks, 0, sizeof(int) * park_count);
	int floor = floor_length;
	while (park_count > 0) {
		best_parks[park_count - 1] = best_cost[floor][park_count].floor;
		floor = best_cost[floor][park_count].floor + 1;
		park_count --;
	}

	// release memory
	for (i = 0; i <= floor_length; i ++) {
		free(best_cost[i]);
	}
	free(best_cost);

	return best_parks;
}

int main(int argc, const char *argv[])
{
	int floor_person_number[] = {0, 2, 3, 2, 9, 2, 1, 7, 2, 4, 2};
	int best_floor_ladder = -1;
//	int best_floor_park = elevator_best_park_use_exhaustion(floor_person_number, 
//		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
//		&best_floor_ladder, 2);
//	int best_floor_park = elevator_best_park_no_exhaustion(floor_person_number, 
//		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
//		&best_floor_ladder, 2);
//	int best_floor_park = elevator_best_park_use_median(floor_person_number, 
//		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
//		&best_floor_ladder, 2);
//	int best_floor_park = elevator_best_park_use_balance(floor_person_number, 
//		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
//		&best_floor_ladder, 2);
//	printf("best floor to park is %d, and its ladder cost %d\n", 
//		best_floor_park+1, best_floor_ladder);

	// best k parks
	int park_count = 2;
	int* best_parks = elevator_best_parks(floor_person_number, 
		sizeof(floor_person_number)/sizeof(floor_person_number[0]), 
		&best_floor_ladder, 1, park_count);
	printf("best k floor to park ladder cost %d, and the best parks are:\n", 
		best_floor_ladder);
	int i = 0;
	for (i = 0; i < park_count; i ++) {
		if (0 != i) printf(", ");
		printf("%d", (best_parks[i]+1));
	}
	printf("\n");
	free(best_parks);
	return 0;
}
