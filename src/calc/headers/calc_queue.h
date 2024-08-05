// Copyright [2023] <Burr Etienne>

#ifndef SRC_CALC_HEADERS_CALC_QUEUE_H_
#define SRC_CALC_HEADERS_CALC_QUEUE_H_

#include "../headers/calc_token.h"

#define CALC_QUEUE_MAX_SIZE (256)

typedef struct calc_token_queue {
  int start;
  int end;
  calc_token data[CALC_QUEUE_MAX_SIZE];
} calc_token_queue;

// Functions for work with queue
void calc_queue_init(calc_token_queue *queue);
void calc_queue_add(calc_token_queue *queue, calc_token token);
calc_token calc_queue_get(calc_token_queue *queue);

#endif  // SRC_CALC_HEADERS_CALC_QUEUE_H_
