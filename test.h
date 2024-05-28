#ifndef READER_LOG_CHECKER_H
#define READER_LOG_CHECKER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 숫자, 공백, 64개의 알파벳, 그리고 널 문자('\0')를 고려한 길이
#define MAX_LINE_LENGTH 128

void check_log_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("파일을 열 수 없습니다");
    exit(EXIT_FAILURE);
  }

  char line[MAX_LINE_LENGTH];
  int line_number = 0;
  while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
    line_number++;

    // 줄의 끝에서 개행 문자 제거
    line[strcspn(line, "\n")] = 0;

    // 숫자와 공백을 건너뛰고 첫 번째 알파벳 위치 찾기
    char *first_alpha = line;
    while (*first_alpha && !isalpha(*first_alpha)) {
      first_alpha++;
    }

    // 알파벳 부분의 길이 확인
    size_t alpha_len = strlen(first_alpha);
    if (alpha_len != 64) { // 알파벳 부분이 64자가 아닌 경우
      printf("경고: %d번 줄은 64개의 알파벳이 아닙니다. [%s]\n", line_number,
             line);
      continue;
    }

    char first_char = *first_alpha; // 첫 번째 알파벳
    int is_same = 1;                // 모든 알파벳이 동일한지 여부
    for (int i = 1; i < 64; i++) {
      if (first_alpha[i] != first_char) {
        is_same = 0;
        break;
      }
    }

    if (!is_same) {
      printf("경고: %d번 줄에 동일하지 않은 알파벳이 있습니다. [%s]\n",
             line_number, line);
    }
  }

  fclose(file);
}

#endif // READER_LOG_CHECKER_H
