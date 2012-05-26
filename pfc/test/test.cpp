#include <pfc/pfc.h>

int main(int argc, char **argv) {
  /* base64.h (and a bit of string stuff) */

  pfc::string8 encoded_string;
  char original_plaintext[] = "This is some plaintext";
  char decoded_string[25];

  pfc::base64_encode(encoded_string, original_plaintext, strlen(original_plaintext));

  printf("# Encoded: %s\n", encoded_string.toString());
  if (strcmp(encoded_string, "VGhpcyBpcyBzb21lIHBsYWludGV4dA==") == 0) {
    printf("ok 1\n");
  } else {
    printf("not ok 1\n");
  }

  memset(decoded_string, 0, sizeof(decoded_string));
  pfc::base64_decode(encoded_string.toString(), decoded_string);
  printf("# Decoded: %s\n", decoded_string);
  if (strcmp(decoded_string, original_plaintext) == 0) {
    printf("ok 2\n");
  } else {
    printf("not ok 2\n");
  }
  

  // threads.cpp
  t_size thread_count;
  thread_count = pfc::getOptimalWorkerThreadCount();

  printf("# thread_count: %d\n", thread_count);
}
