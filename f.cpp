#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Item {
    char name[100];
    float quantity;
    float unit_price;
};

struct Invoice {
    char from_who[100];
    char to_who[100];
    char logo[100];
    char number[100];
    char date[100];
    char due_date[100];
    struct Item items[10];  // Assuming a maximum of 10 items per invoice
    char notes[1000];
};

struct CSVParser {
    char csv_name[100];
};

void get_array_of_invoices(struct CSVParser *csv_parser, struct Invoice *invoices, int *num_invoices) {
    FILE *file = fopen(csv_parser->csv_name, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, sizeof(line), file);  // Skip header

    while (fgets(line, sizeof(line), file) != NULL) {
        struct Invoice invoice;
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]",
               invoice.from_who, invoice.to_who, invoice.logo, invoice.number,
               invoice.date, invoice.due_date, invoice.items, invoice.notes);

        // Parse items
        char *token = strtok(invoice.items, ";");
        int item_index = 0;
        while (token != NULL) {
            sscanf(token, "%[^:]:%f:%f", invoice.items[item_index].name,
                   &invoice.items[item_index].quantity, &invoice.items[item_index].unit_price);
            token = strtok(NULL, ";");
            item_index++;
        }

        invoices[*num_invoices] = invoice;
        (*num_invoices)++;
    }

    fclose(file);
}

struct ApiConnector {
    char url[100];
    char invoices_directory[100];
};

void connect_to_api_and_save_invoice_pdf(struct ApiConnector *api, struct Invoice *invoice) {
    // Implementation of API connection and saving PDF in C would be more complex
    printf("Connecting to API for %s invoice...\n", invoice->number);
    // Simulated success
    printf("File Saved\n");
}

int main(int argc, char *argv[]) {
    struct CSVParser csv_parser;
    struct Invoice invoices[100];  // Assuming a maximum of 100 invoices
    int num_invoices = 0;

    if (argc > 1) {
        strncpy(csv_parser.csv_name, argv[1], sizeof(csv_parser.csv_name));
    } else {
        strncpy(csv_parser.csv_name, "invoices.csv", sizeof(csv_parser.csv_name));
    }

    get_array_of_invoices(&csv_parser, invoices, &num_invoices);

    struct ApiConnector api;
    strncpy(api.url, "https://invoice-generator.com", sizeof(api.url));
    strncpy(api.invoices_directory, "./invoices", sizeof(api.invoices_directory));

    for (int i = 0; i < num_invoices; i++) {
        connect_to_api_and_save_invoice_pdf(&api, &invoices[i]);
    }

    return 0;
}
