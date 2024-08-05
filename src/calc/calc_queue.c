// Copyright [2023] <Burr Etienne>

#include "headers/calc_queue.h"

void calc_queue_init(calc_token_queue *queue) {
  queue->start = 0;
  queue->end = -1;
}

void calc_queue_add(calc_token_queue *queue, calc_token token) {
  if (queue->end < CALC_QUEUE_MAX_SIZE) {
    queue->end = queue->end + 1;
    queue->data[queue->end] = token;
  }
}

calc_token calc_queue_get(calc_token_queue *queue) {
  calc_token token = {};

  token.type = CALC_TOKEN_NUMBER_TYPE_CODE;
  token.calc_data.number = NAN;

  if (queue->start <= queue->end) {
    token = queue->data[queue->start];
    queue->start = queue->start + 1;
  }
  return token;
}
