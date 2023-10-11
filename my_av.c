// Copyright Similea Alin-Andrei 314CA 2022-2023
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NMAX_LINE 1000

// ================
// TASK 1 FUNCTIONS
// ================
int endings(char *url)
{
	char ending1[] = ".exe";
	int length = strlen(url);
	int kon = 0;
	for (int i = 0; i < strlen(ending1); i++) {
		if (url[length - i - 1] == ending1[3 - i])
			kon++;
	}
	if (kon == 4)
		return 1;  // ends with ".exe" - bad

	char ending2[] = ".bin";
	kon = 0;
	for (int i = 0; i < strlen(ending2); i++) {
		if (url[length - i - 1] == ending2[3 - i])
			kon++;
	}
	if (kon == 4)
		return 1;  // ends with ".bin" - bad
	return 0;
}

int database_bad(char *url)
{
	char file_path[] = "data/urls/domains_database";
	FILE *database = fopen(file_path, "rt");
	char bad_url[NMAX_LINE];
	int len;
	while (fgets(bad_url, NMAX_LINE, database)) {
		bad_url[strlen(bad_url) - 1] = '\0';
		char *b = strstr(url, bad_url);
		if (b) {
			fclose(database);
			return 1;
		}
	}
	fclose(database);
	return 0;
}

int digits_domain(char *url)
{
	char delim[] = "/";
	char *domain = strtok(url, delim);

	int nr_digits = 0;

	for (int i = 0; i < strlen(domain); i++)
		if (isdigit(domain[i]))
			nr_digits++;

	if (nr_digits >= (double)strlen(domain) * 0.1)
		return 1;  // too many digits - bad

	return 0;
}

// ================
// TASK 2 FUNCTIONS
// ================
int flow_time(char *traffic, char *delim)
{
	// Flow_duration is the 5th element on a line
	char *flow_duration = strtok(traffic, delim);
	for (int i = 1; i <= 4; i++)
		flow_duration = strtok(NULL, delim);

	char delim_flow[] = " :";
	char *element = strtok(flow_duration, delim_flow);	// number days
	double nr = atof(element);
	if (nr >= 1)
		return 1;

	element = strtok(NULL, delim_flow);	 // skip "days" word

	for (int i = 1; i <= 3; i++) {
		element = strtok(NULL, delim_flow);	 // hours, minutes, seconds
		nr = atof(element);
		if (nr >= 1)
			return 1;
	}

	return 0;
}

int flow_pkts(char *traffic, char *delim)
{
	char *pkts = strtok(traffic, delim);
	char *aux;
	while (pkts) {
		// At the last iteration, pkts = NULL, so we
		// have to keep the previous "pkts" value in an "aux";
		aux = pkts;
		pkts = strtok(NULL, delim);
	}
	pkts = aux;
	pkts[strlen(pkts) - 1] = '\0';
	int nr = atof(pkts);
	if (nr > 0)
		return 1;  // bad
	return 0;	   // ok
}

int main(void)
{
	// ======
	// TASK 1
	// ======

	char url[NMAX_LINE];
	FILE *in_1 = fopen("data/urls/urls.in", "rt");
	FILE *out_1 = fopen("urls-predictions.out", "wt");

	while (fgets(url, NMAX_LINE, in_1)) {
		url[strlen(url) - 1] = '\0';  // the last character would've been "\n"
		if (endings(url) == 1) {
			fprintf(out_1, "1\n");
			continue;
		}
		if (database_bad(url) == 1) {
			fprintf(out_1, "1\n");
			continue;
		}
		if (digits_domain(url) == 1) {
			fprintf(out_1, "1\n");
			continue;
		}
		fprintf(out_1, "0\n");
	}

	fclose(in_1);
	fclose(out_1);

	// ======
	// TASK 2
	// ======
	char traffic[NMAX_LINE];
	char delim[] = ",";	 // to separate the elements on a line

	FILE *in_2 = fopen("data/traffic/traffic.in", "rt");
	FILE *out_2 = fopen("traffic-predictions.out", "wt");
	fgets(traffic, NMAX_LINE, in_2);  // skip first line

	while (fgets(traffic, NMAX_LINE, in_2)) {
		traffic[strlen(traffic) - 1] = '\0';

		char traffic_copy[NMAX_LINE];
		strcpy(traffic_copy, traffic);
		if (flow_time(traffic, delim) == 1) {
			if (flow_pkts(traffic_copy, delim) == 0) {
				fprintf(out_2, "0\n");
				continue;
			}
			fprintf(out_2, "1\n");
			continue;
		}
		fprintf(out_2, "0\n");
	}
	fclose(in_2);
	fclose(out_2);
	return 0;
}
