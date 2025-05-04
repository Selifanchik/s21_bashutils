#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int main(int argc, const char* argv[]) {
	regex_t regex;
	char* pattern = "a\\+b";
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        char errbuf[100];
        regerror(ret, &regex, errbuf, sizeof(errbuf));
        printf("Error: %s\n", errbuf);
        return 1;
    }

	const char* text = argv[1];
	regmatch_t match;
	int exec_status = regexec(&regex, text, 1, &match, 0);
	if (exec_status == 0) {
		printf("match\n");
		printf("rm.so = %d, rm_eo = %d\n", match.rm_so, match.rm_eo);
	} else {
		printf("no match\n");
	}
	regfree(&regex);
	return 0;
}