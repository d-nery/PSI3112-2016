#include <stdio.h>

int main() {
	FILE* in1 = fopen("dadosch144def500hz.1.txt", "r");
	FILE* in2 = fopen("dadosch244def500hz.2.txt", "r");
	FILE* out = fopen("out.txt", "w");

	float num;
	int i = 128;

	fprintf(out, "dados1 = { {\n");
	while (i--) {
		fscanf(in1, "%f", &num);
		fprintf(out, "\t%f,\n", num);
		fscanf(in1, "%f", &num);
	}
	fclose(in1);
	in1 = fopen("dadosch144def500hz.1.txt", "r");
	fprintf(out, "}, {\n");
	i = 128;
	while (i--) {
		fscanf(in1, "%f", &num);
		fscanf(in1, "%f", &num);
		fprintf(out, "\t%f,\n", num);
	}
	fprintf(out, "} };\n");

	i = 128;

	fprintf(out, "\ndados2 = { {\n");
	while (i--) {
		fscanf(in2, "%f", &num);
		fprintf(out, "\t%f,\n", num);
		fscanf(in2, "%f", &num);
	}
	fclose(in2);
	in2 = fopen("dadosch244def500hz.2.txt", "r");
	fprintf(out, "}, {\n");
	i = 128;
	while (i--) {
		fscanf(in2, "%f", &num);
		fscanf(in2, "%f", &num);
		fprintf(out, "\t%f,\n", num);
	}
	fprintf(out, "} };\n");

	fclose(in1);
	fclose(in2);
	fclose(out);

	return 0;
}
