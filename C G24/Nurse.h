#ifndef Nurse_H
#define Nurse_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "Doctor.h"

typedef struct 
{
    char ID[256];
    char med_name[256];
    char indication[256];
    char quantity[256];
    char price[256];
    char exp_date[256];
    char alert[256];
}Med_Info;

typedef struct {
    char allergy[50];
    int count;
} allergycount;

void currentDateYearMonth(int *year, int *month)
{
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    *year = local_time->tm_year + 1900;
    *month = local_time->tm_mon +1;
}

bool medExpired(int exp_month, int exp_year)
{
    int current_year, current_month;
    currentDateYearMonth(&current_year, &current_month);

    if(exp_year < current_year || (exp_year == current_year && exp_month < current_month))
    {   
        return true;
    }
    return false;
}

bool over5years(int exp_year)
{
    int current_year, current_month;
    currentDateYearMonth(&current_year, &current_month);

    int Max = exp_year - current_year;
    if(Max > 5)
    {
        return true;
    }
    return false;
}

void CopyIntoInventoryArray(Med_Info medicineInfo[256], int *size) 
{
    char buffer[256];
    int i = 0;
    FILE *inventory = fopen("Inventory.txt", "r");
    if (inventory == NULL) {
        printf("Error opening Inventory.txt file.\n");
        return;
    }

    while (fgets(buffer, 256, inventory) != NULL) {
        // Remove newline character
        buffer[strcspn(buffer, "\n")] = '\0';

        char *token = strtok(buffer, ",");
        int info = 0;
        while (token != NULL && info < 6) {
            switch (info) {
                case 0:
                    strcpy(medicineInfo[i].ID, token);
                    break;
                case 1:
                    strcpy(medicineInfo[i].med_name, token);
                    break;
                case 2:
                    strcpy(medicineInfo[i].indication, token);
                    break;
                case 3:
                    strcpy(medicineInfo[i].quantity, token);
                    break;
                case 4:
                    strcpy(medicineInfo[i].price, token);
                    break;
                case 5:
                    strcpy(medicineInfo[i].exp_date, token);
                    break;
                case 6:
                    strcpy(medicineInfo[i].alert, token);
                    break;
            }
            token = strtok(NULL, ",");
            info++;
        }
        i++;
    }
    *size = i;
    fclose(inventory);
}

void uploadtoFile(Med_Info medicineInfo[256], int size)
{
    FILE *pF = fopen("Inventory.txt", "w");
    for(int i = 0; i < size && medicineInfo[i].med_name[0] != '\0'; i++)
    {
        fprintf(pF, "%s,%s,%s,%s,%s,%s,%s,\n",
                medicineInfo[i].ID,
                medicineInfo[i].med_name,
                medicineInfo[i].indication,
                medicineInfo[i].quantity,
                medicineInfo[i].price,
                medicineInfo[i].exp_date,
                medicineInfo[i].alert
                );
    }
    fclose(pF);
}

int DoctorScheduler(char *id)
{
    schedulestruct DocSchedule[256];
    char buffer[256];
    int i = 0;

    FILE *schedule = fopen("schedule.txt", "r");

    while (fgets(buffer, sizeof(buffer), schedule) != NULL) 
    { 
        buffer[strcspn(buffer, "\n")] = '\0';
        char *token = strtok(buffer, ",");
        strcpy(DocSchedule[i].doctorid,token);

        token = strtok(NULL,",");
        strcpy(DocSchedule[i].name,token);
        
        token = strtok(NULL,",");
        strcpy(DocSchedule[i].day,token);

        token = strtok(NULL,",");
        strcpy(DocSchedule[i].timeslot,token);

        token = strtok(NULL,",");
        strcpy(DocSchedule[i].availability,token);

        token = strtok(NULL,",");
        strcpy(DocSchedule[i].nurseid,token);
        i++;
    }
    fclose(schedule);

    int scheduleinput;
    int c;
    char DocID[100];
    int valid = 0;
    int slot = 1;

    while(1)
    {
        printf("\n<<Schedule>>\n1. View All Doctor\n2. Search Specific Doctor\n3. View Nurse Schedule(Own)\n4. Quit\n");
        printf("Enter your option: ");

        if(scanf("%d", &scheduleinput) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
        {
            printf("Please enter an integer!\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        while ((c = getchar()) != '\n' && c != EOF);
        int ScheduleSize = sizeof(DocSchedule)/ sizeof(DocSchedule[0]);
        
        switch (scheduleinput)
        {
        case 1:
            for (int i = 0; i < ScheduleSize && DocSchedule[i].name[0] != '\0'; i++) {
                printf("===================================\n");
                printf("|Slot        :%-20d|\n",i+1);
                printf("|Doctor ID   :%-20s|\n",DocSchedule[i].doctorid);
                printf("|Name        :%-20s|\n",DocSchedule[i].name);
                printf("|Day         :%-20s|\n",DocSchedule[i].day);
                printf("|Timeslot    :%-20s|\n",DocSchedule[i].timeslot);
                printf("|Availability:%-20s|\n",DocSchedule[i].availability);
                printf("|Nurse ID    :%-20s|\n",DocSchedule[i].nurseid);
            }
            printf("===================================\n");
            break;
        case 2:           
            printf("\n<<Search Doctor>>\n");
            printf("Enter doctor ID: ");
            fgets(DocID, sizeof(DocID), stdin);
            DocID[strcspn(DocID, "\n")] = '\0';
            DocID[0] = toupper(DocID[0]);

            if(strlen(DocID) != 4)
            {
                printf("Please enter a valid ID format!(MXXX or SXXX)\n");
                break;
            }

            bool doctor = false;
            for(int i = 0; i < ScheduleSize && DocSchedule[i].doctorid[0] != '\0'; i++)
            {
                if(strcmp(DocSchedule[i].doctorid, DocID) == 0)
                {
                    printf("===================================\n");
                    printf("|Slot        :%-20d|\n",i+1);
                    printf("|Doctor ID   :%-20s|\n",DocSchedule[i].doctorid);
                    printf("|Name        :%-20s|\n",DocSchedule[i].name);
                    printf("|Day         :%-20s|\n",DocSchedule[i].day);
                    printf("|Timeslot    :%-20s|\n",DocSchedule[i].timeslot);
                    printf("|Availability:%-20s|\n",DocSchedule[i].availability);
                    printf("|Nurse ID    :%-20s|\n",DocSchedule[i].nurseid);
                    doctor = true;
                }
            }
            if(doctor)
            {
                printf("===================================\n");
            }
            if(!doctor)
            {
                printf("Doctor not exist!\n");
            }
            break;
        case 3:
            // int valid = 0;
            // int slot = 1;

            for(int i = 0; i < ScheduleSize ; i++)
            {
                if(strcmp(id, DocSchedule[i].nurseid) == 0)
                {
                    valid = 1;
                }
            }

            if(!valid)
            {
                printf("You do not have any schedule!\n");
                break;
            }

            printf("===================================\n");
            printf("           Nurse Schedule          \n");
            for(int i = 0; i < ScheduleSize && DocSchedule[i].nurseid[0] != '\0'; i++)
            {   
                if(strcmp(id,DocSchedule[i].nurseid) == 0)
                {
                    printf("===================================\n");
                    printf("|Slot        :%-20d|\n",slot++);
                    printf("|Nurse ID    :%-20s|\n",DocSchedule[i].nurseid);
                    printf("|Doctor ID   :%-20s|\n",DocSchedule[i].doctorid);
                    printf("|Name        :%-20s|\n",DocSchedule[i].name);
                    printf("|Day         :%-20s|\n",DocSchedule[i].day);
                    printf("|Timeslot    :%-20s|\n",DocSchedule[i].timeslot);
                    printf("|Availability:%-20s|\n",DocSchedule[i].availability);
                    valid = 1;
                }
            }
            if(valid)
            {
                printf("===================================\n");
            }
            break;
        case 4:
            printf("Quiting...\n");
            return 0;
        default:
            printf("Invalid option!\n");
            break;
        }
    }
}

void InventoryManagement(Med_Info medicineInfo[256], int *size)
{
    //Access and manage SUPPLIES and MEDICATIONS at the nurse's station directly through the HMS. 
    //if stock < certain lvl then print low stock!
    char category[50][50] = {"ID","Name","Indication","Qty.","Price","Exp_Date","Alert"}; 
    int input;
    int c;
    bool noShortage = true;    
    int combinedQuantity;
    bool LowStockNonExpired = false;

    while(1)
    {
        printf("\n<<Inventory Management>>\n");
        printf("1. View Supplies\t4. Add Supplies\n2. View Medications\t5. Add Medications\n3. Restock\t\t6. Quit\n");
        printf("Enter your option: ");

        if(scanf("%d", &input) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
        {
            printf("Please enter an integer!\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        while ((c = getchar()) != '\n' && c != EOF);
        switch (input)
        {
        case 1:
            printf("<<Supplies>>\n");
            printf("-----------------------------------------------------------------\n");
            printf("|%-5s| %-20s| %-5s| %-5s| %-10s| %-8s|\n", 
                    category[0],
                    category[1],
                    category[3],
                    category[4],
                    category[5],
                    category[6]);

            for (int i = 0; i < *size; i++) 
            {
                int combinedQuantity = atoi(medicineInfo[i].quantity);
                for (int j = i + 1; j < *size; j++) 
                {
                    if (strcmp(medicineInfo[i].med_name, medicineInfo[j].med_name) == 0) 
                    {
                        combinedQuantity += atoi(medicineInfo[j].quantity);
                    }
                }

                // Update alert status based on combined quantity
                if (combinedQuantity < 10 && combinedQuantity > 0)
                {
                    strcpy(medicineInfo[i].alert, "Low");
                } 
                else 
                {
                    strcpy(medicineInfo[i].alert, "-");
                }
            }
            uploadtoFile(medicineInfo, *size);

            for(int i = 0; i < *size; i++)
            {
                int quantity = atoi(medicineInfo[i].quantity);//convert to int

                int SupExpMonth, SupExpYear;
                sscanf(medicineInfo[i].exp_date, "%d/%d", &SupExpMonth,&SupExpYear);

                if(medicineInfo[i].ID[0] == 'S' && quantity > 0 && !medExpired(SupExpMonth,SupExpYear))//can use strncmp also strncmp(medicineInfo[j].ID, "S", 1)
                {
                    printf("-----------------------------------------------------------------\n");
                    printf("|%-5s| %-20s| %-5s| %-5s| %-10s| %-8s|\n", 
                            medicineInfo[i].ID,
                            medicineInfo[i].med_name,
                            medicineInfo[i].quantity,
                            medicineInfo[i].price,
                            medicineInfo[i].exp_date,
                            medicineInfo[i].alert);
                }
            }
            printf("-----------------------------------------------------------------\n");
            break;
        case 2:
            printf("<<Medications>>\n");
            printf("----------------------------------------------------------------------------------\n");
            printf("|%-5s| %-20s| %-15s| %-5s| %-5s| %-10s| %-8s|\n", 
                    category[0],
                    category[1],
                    category[2],
                    category[3],
                    category[4],
                    category[5],
                    category[6]);

            for (int i = 0; i < *size; i++) 
            {
                int combinedQuantity = atoi(medicineInfo[i].quantity);
                for (int j = i + 1; j < *size; j++) 
                {
                    if (strcmp(medicineInfo[i].med_name, medicineInfo[j].med_name) == 0) 
                    {
                        combinedQuantity += atoi(medicineInfo[j].quantity);
                    }
                }

                // Update alert status based on combined quantity
                if (combinedQuantity < 10 && combinedQuantity > 0)
                {
                    strcpy(medicineInfo[i].alert, "Low");
                } 
                else 
                {
                    strcpy(medicineInfo[i].alert, "-");
                }
            }
            uploadtoFile(medicineInfo, *size);

            for(int i = 0; i < *size; i++)
            {
                int quantity = atoi(medicineInfo[i].quantity);

                int MedExpMonth, MedExpYear;
                sscanf(medicineInfo[i].exp_date, "%d/%d", &MedExpMonth,&MedExpYear);

                if(medicineInfo[i].ID[0] == 'M' && quantity > 0 && !medExpired(MedExpMonth,MedExpYear))
                {
                    printf("----------------------------------------------------------------------------------\n");
                    printf("|%-5s| %-20s| %-15s| %-5s| %-5s| %-10s| %-8s|\n", 
                            medicineInfo[i].ID,
                            medicineInfo[i].med_name,
                            medicineInfo[i].indication,
                            medicineInfo[i].quantity,
                            medicineInfo[i].price,
                            medicineInfo[i].exp_date,
                            medicineInfo[i].alert);
                }
            }
            printf("----------------------------------------------------------------------------------\n");
            break;
            uploadtoFile(medicineInfo, *size);
        case 3:
            for(int i = 0; i < *size; i++)
            {
                combinedQuantity = atoi(medicineInfo[i].quantity);
                for (int j = i + 1; j < *size; j++) 
                {
                    if (strcmp(medicineInfo[i].med_name, medicineInfo[j].med_name) == 0) 
                    {
                        combinedQuantity += atoi(medicineInfo[j].quantity);
                    }
                }

                if (combinedQuantity < 10 && combinedQuantity > 0) 
                {   
                    strcpy(medicineInfo[i].alert, "Low");
                    noShortage = false;  // Found a quantity below the threshold
                }
                else
                {
                    strcpy(medicineInfo[i].alert, "-");
                }
            }
            uploadtoFile(medicineInfo, *size);

            for (int i = 0; i < *size; i++) 
            {
                int RestockMonth, RestockYear;
                sscanf(medicineInfo[i].exp_date, "%d/%d", &RestockMonth,&RestockYear);

                if (strcmp(medicineInfo[i].alert, "Low") == 0 && !medExpired(RestockMonth, RestockYear))
                {
                    LowStockNonExpired = true;
                    break;
                }
            }

            if(noShortage || !LowStockNonExpired)
            {
                printf("There are no shortages in the inventory!\n");
                break;
            }
            else
            {
                printf("<<Low Stock>>\n");
                printf("-------------------------------------------------------\n");
                printf("|%-5s| %-20s| %-5s| %-5s| %-10s|\n", 
                        category[0],
                        category[1],
                        category[3],
                        category[4],
                        category[5]);

                for (int i = 0; i < *size; i++) 
                {
                    int RestockMonth, RestockYear;
                    sscanf(medicineInfo[i].exp_date, "%d/%d", &RestockMonth, &RestockYear);
                    if(strcmp(medicineInfo[i].alert, "Low") == 0 && !medExpired(RestockMonth, RestockYear))
                    {
                        printf("-------------------------------------------------------\n");
                        printf("|%-5s| %-20s| %-5s| %-5s| %-10s|\n", 
                                medicineInfo[i].ID,
                                medicineInfo[i].med_name,
                                medicineInfo[i].quantity,
                                medicineInfo[i].price,
                                medicineInfo[i].exp_date);
                    }
                }
                printf("-------------------------------------------------------\n");
            }
            
            
            int userinput;
            char inputId[256];
            int userQuantity;
            int exp_month, exp_year;
            char exp_date[256];
            char NewSuppliesName[60];
            int SupQuantity;
            int SupPrice;
            int SupExpMonth, SupExpYear;
            char NewSupExpDate[50];
            int SupExist = 0;
            char NewMedName[60];
            int MedQuantity;
            char MedIndication[50];
            int MedPrice;
            int MedExpMonth, MedExpYear;
            char NewMedExpDate[50];
            int MedExist = 0;

            while(1)
            {
                printf("\nDo you want to: \n1. Restock\n2. Quit\n");
                printf("Enter option: ");
                if(scanf("%d", &userinput) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                {
                    printf("Please enter an integer!\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    continue;
                }

                while ((c = getchar()) != '\n' && c != EOF);
                if(userinput == 1)//enter id, name and exp date? if name and exp date same then just apppend qty, else add into new line
                {
                    printf("Enter ID to restock: ");
                    fgets(inputId, sizeof(inputId), stdin);      
                    inputId[strcspn(inputId, "\n")] = '\0';  
                    inputId[0] = toupper(inputId[0]);

                    if(strlen(inputId) != 4)
                    {
                        printf("Please enter a valid ID format!(MXXX or SXXX)\n");
                        break;
                    }

                    int found = 0;
                    bool NameMatch = false;
                    for(int i = 0; i < *size; i++)
                    {
                        if(strcmp(medicineInfo[i].ID,inputId) == 0 && strcmp(medicineInfo[i].alert,"Low") == 0)
                        {
                            NameMatch = true;
                            do
                            {
                                printf("Enter quantity: ");
                                if(scanf("%d", &userQuantity) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                                {
                                    printf("Please enter an integer!\n");
                                    while ((c = getchar()) != '\n' && c != EOF);
                                    continue;
                                }

                                if(userQuantity < 10 || userQuantity > 2000)
                                {
                                    printf("Please enter a valid amount between 10 - 2000!\n");
                                }
                            }while(userQuantity < 10 || userQuantity > 2000);

                            do
                            {
                                printf("Enter expired month(MM): ");
                                if(scanf("%d", &exp_month) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                                {
                                    printf("Please enter an integer!\n");
                                    while ((c = getchar()) != '\n' && c != EOF);
                                    continue;
                                }

                                if(exp_month <= 0 || exp_month >12)
                                {
                                    printf("Please enter a valid month!\n");
                                }
                            } while (exp_month <= 0 || exp_month >12);
                            
                            
                            do
                            {
                                printf("Enter expired year(YYYY): ");
                                if(scanf("%d", &exp_year) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                                {
                                    printf("Please enter an integer!\n");
                                    while ((c = getchar()) != '\n' && c != EOF);
                                    continue;
                                }

                                if(exp_year <= 1000 || exp_year >= 9999)
                                {
                                    printf("Please enter a valid year!\n");
                                }
                            } while (exp_year <= 1000 || exp_year >= 9999);
                            
                            
                            if(medExpired(exp_month, exp_year))
                            {
                                printf("The expired date you entered already passed!\n");
                                break;
                            }
                            else if(over5years(exp_year))
                            {
                                printf("Please enter expired date within these 5 years!");
                                break;
                            }
                            else
                            {
                                sprintf(exp_date, "%02d/%04d", exp_month, exp_year);
                            }

                            if(strcmp(medicineInfo[i].exp_date,exp_date) == 0)
                            {
                                int newquantity = userQuantity + atoi(medicineInfo[i].quantity);
                                if(newquantity > 10)
                                {
                                    strcpy(medicineInfo[i].alert, "-");
                                }
                                else
                                {
                                    strcpy(medicineInfo[i].alert, "Low");
                                }

                                sprintf(medicineInfo[i].quantity, "%d", newquantity);
                                printf("Restock successful!\n");
                                found = 1;
                                break;
                            }
                            if(!found)
                            {   
                                srand(time(0));
                                int randomID = rand() % 1000; //0-999 random
                                int combineQuantity = userQuantity + atoi(medicineInfo[i].quantity);

                                if(strcmp(medicineInfo[i].ID,inputId) == 0)
                                {   
                                    if(medicineInfo[i].ID[0] == 'S')
                                    {
                                        sprintf(medicineInfo[*size].ID, "S%03d", randomID);
                                    }
                                    else
                                    {
                                        sprintf(medicineInfo[*size].ID, "M%03d", randomID);
                                    }
                                    strcpy(medicineInfo[*size].med_name, medicineInfo[i].med_name);
                                    strcpy(medicineInfo[*size].indication, medicineInfo[i].indication);
                                    sprintf(medicineInfo[*size].quantity, "%d", userQuantity);
                                    strcpy(medicineInfo[*size].price, medicineInfo[i].price);
                                    strcpy(medicineInfo[*size].exp_date, exp_date);
                                    if(combineQuantity > 10)
                                    {
                                        strcpy(medicineInfo[*size].alert, "-");
                                    }
                                    else
                                    {
                                        strcpy(medicineInfo[*size].alert, "Low");
                                    }
                                    (*size)++;
                                    printf("New entry added successfully!\n");
                                    break;
                                } 
                            }      
                        }
                        else if (strcmp(medicineInfo[i].ID,inputId) == 0 && strcmp(medicineInfo[i].alert,"Low") != 0)
                        {
                            NameMatch = true;
                            printf("This stock is sufficient and no need restock!\n");
                        }              
                    }
                    if(!NameMatch)
                    {
                        printf("Does not exists in inventory!\n");
                    }
                    uploadtoFile(medicineInfo,*size);
                    break;
                }
                else if(userinput == 2)//quit 
                {
                    printf("Quiting...\n");
                    break;
                }
                else
                {
                    printf("Invalid input!\n");
                }
            }
            break;
        case 4:
            printf("\n<<Add Supplies>>\n");
            do
            {
                printf("Enter supplies name: ");
                fgets(NewSuppliesName, sizeof(NewSuppliesName), stdin);
                NewSuppliesName[strcspn(NewSuppliesName, "\n")] = '\0';

                if(strlen(NewSuppliesName) == 0)
                {
                    printf("Supply name cannot be empty!\n");
                }
                else if(strlen(NewSuppliesName) > 49)
                {
                    printf("Max word count is 50!\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                }
                
            } while (strlen(NewSuppliesName) == 0 || strlen(NewSuppliesName) > 49);
            

            for(int i = 0; i < *size; i++)
            {
                if(strcmp(medicineInfo[i].med_name, NewSuppliesName) == 0)
                {
                    printf("This supply already exists in our inventory!\n");
                    SupExist = 1;
                    break;
                }
            }

            if(!SupExist)
            {
                do
                {
                    printf("Enter quantity: ");
                    if(scanf("%d", &SupQuantity) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(SupQuantity < 50)
                    {
                        printf("The minimum quantity to add is 50!\n");
                    }
                    else if(SupQuantity > 2000)
                    {
                        printf("Please enter quantity below 2000!\n");
                    }
                } while (SupQuantity < 50 || SupQuantity > 2000);

                do
                {
                    printf("Enter price: ");
                    if(scanf("%d", &SupPrice) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(SupPrice <= 0)
                    {
                        printf("Please enter a valid amount!\n");
                    }
                    else if(SupPrice > 1000)
                    {
                        printf("Price is too high!\nEnter an affordable price below 1000!\n");
                    }
                } while (SupPrice <= 0 || SupPrice > 1000);
                
                do
                {
                    printf("Enter expired month(MM): ");
                    if(scanf("%d", &SupExpMonth) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(SupExpMonth <= 0 || SupExpMonth >12)
                    {
                        printf("Please enter a valid month!\n");
                    }
                } while (SupExpMonth <= 0 || SupExpMonth >12);

                do
                {
                    printf("Enter expired year(YYYY): ");
                    if(scanf("%d", &SupExpYear) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(SupExpYear <= 1000 || SupExpYear >= 9999)
                    {
                        printf("Please enter a valid year!\n");
                    }
                } while (SupExpYear <= 1000 || SupExpYear >= 9999);
                
                
                if(medExpired(SupExpMonth, SupExpYear))
                {
                    printf("The expired date you entered already passed!\n");
                    break;
                }
                else if(over5years(SupExpYear))
                {
                    printf("Please enter expired date within these 5 years!\n");
                    break;
                }
                else
                {
                    sprintf(NewSupExpDate, "%02d/%04d", SupExpMonth, SupExpYear);
                }

                srand(time(0));
                int SupRandomID = rand() % 1000; //0-999 random

                sprintf(medicineInfo[*size].ID, "S%03d", SupRandomID);
                strcpy(medicineInfo[*size].med_name, NewSuppliesName);
                strcpy(medicineInfo[*size].indication, " ");
                sprintf(medicineInfo[*size].quantity, "%d", SupQuantity);
                sprintf(medicineInfo[*size].price, "%d", SupPrice);
                strcpy(medicineInfo[*size].exp_date, NewSupExpDate);
                strcpy(medicineInfo[*size].alert, "-");
                (*size)++;
                printf("Supplies added successful!\n");
                uploadtoFile(medicineInfo, *size);
                break;
            }
            break;
        case 5:
            printf("\n<<Add Medications>>\n");

            do
            {
                printf("Enter medications name: ");
                fgets(NewMedName, sizeof(NewMedName), stdin);
                NewMedName[strcspn(NewMedName, "\n")] = '\0';

                if(strlen(NewMedName) == 0)
                {
                    printf("Medication name cannot be empty!");
                }
                else if(strlen(NewMedName) > 50)
                {
                    printf("Max word count is 50!\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                }

            }while (strlen(NewMedName) == 0 || strlen(NewMedName) > 50);
            
            for(int i = 0; i < *size; i++)
            {
                if(strcmp(medicineInfo[i].med_name, NewMedName) == 0)
                {
                    printf("This mediccation already exists in our inventory!\n");
                    MedExist = 1;
                    break;
                }
            }

            if(!MedExist)
            {
                do
                {
                    printf("Enter indication(10 word count): ");
                    scanf("%s", MedIndication);

                    if(strlen(MedIndication) > 10)
                    {
                        printf("Please enter indication within 10 words!\n");
                    }
                } while (strlen(MedIndication) > 10);
                
                do
                {
                    printf("Enter quantity: ");
                    if(scanf("%d", &MedQuantity) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(MedQuantity < 50)
                    {
                        printf("The minimum quantity to add is 50!\n");
                    }
                    else if(MedQuantity > 2000)
                    {
                        printf("Please enter quantity below 2000!\n");
                    }
                } while (MedQuantity < 50 || MedQuantity > 2000);

                do
                {
                    printf("Enter price: ");
                    if(scanf("%d", &MedPrice) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(MedPrice <= 0)
                    {
                        printf("Please enter a valid amount!\n");
                    }
                    else if(MedPrice > 1000)
                    {
                        printf("Price is too high!\nEnter an affordable price below 1000!\n");
                    }
                } while (MedPrice <= 0 || MedPrice > 1000);
                
                do
                {
                    printf("Enter expired month(MM): ");
                    if(scanf("%d", &MedExpMonth) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(MedExpMonth <= 0 || MedExpMonth >12)
                    {
                        printf("Please enter a valid month!\n");
                    }
                } while (MedExpMonth <= 0 || MedExpMonth >12);

                do
                {
                    printf("Enter expired year(YYYY): ");
                    if(scanf("%d", &MedExpYear) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
                    {
                        printf("Please enter an integer!\n");
                        while ((c = getchar()) != '\n' && c != EOF);
                        continue;
                    }

                    if(MedExpYear <= 1000 || MedExpYear >= 9999)
                    {
                        printf("Please enter a valid year!\n");
                    }
                } while (MedExpYear <= 1000 || MedExpYear >= 9999);
                
                
                if(medExpired(MedExpMonth, MedExpYear))
                {
                    printf("The expired date you entered already passed!\n");
                    break;
                }
                else if(over5years(MedExpYear))
                {
                    printf("Please enter expired date within these 5 years!\n");
                    break;
                }
                else
                {
                    sprintf(NewMedExpDate, "%02d/%04d", MedExpMonth, MedExpYear);
                }

                srand(time(0));
                int MedRandomID = rand() % 1000; //0-999 random

                sprintf(medicineInfo[*size].ID, "M%03d", MedRandomID);
                strcpy(medicineInfo[*size].med_name, NewMedName);
                strcpy(medicineInfo[*size].indication, MedIndication);
                sprintf(medicineInfo[*size].quantity, "%d", MedQuantity);
                sprintf(medicineInfo[*size].price, "%d", MedPrice);
                strcpy(medicineInfo[*size].exp_date, NewMedExpDate);
                (*size)++;
                printf("Medications added successful!\n");
                uploadtoFile(medicineInfo, *size);
                break;
            }
            break;
        case 6:
            printf("Quiting...\n");
            return;
        default:
            printf("Please enter valid option!\n");
        }  
    }
}

void printAllergyReport(allergycount allergyCounts[256], int allergyIndex) {
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("\t\t\t\t\tCommon Allergies Report\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("\tAllergy\t\t\tCount\n");
    printf("-------------------------------------------------------------------------------------------------------------\n");


    // Find the maximum count
    int maxCount = 0;
    for (int i = 0; i < allergyIndex; i++) {
        if (allergyCounts[i].count > maxCount) {
            maxCount = allergyCounts[i].count;
        }
    }

    // Print allergies with the maximum count
    for (int i = 0; i < allergyIndex; i++) {
        if (allergyCounts[i].count == maxCount) {
           printf("\t%-20s      %d\n", allergyCounts[i].allergy, allergyCounts[i].count);
           printf("\tImprovement Suggestions:\n");
            if (strcmp(allergyCounts[i].allergy, "Food Allergy") == 0) {
                
                printf("\t1. Educate the patient on identifying and avoiding foods that trigger their allergy.\n");
                printf("\t2. Encourage reading ingredient labels on food products.\n");
                printf("\t3. Suggest carrying emergency medication, such as an epinephrine auto-injector.\n");
                printf("\t4. Refer the patient to a nutritionist for dietary planning.\n");
                printf("\t5. Provide information on how to handle accidental exposures.\n");
                printf("-------------------------------------------------------------------------------------------------------------\n");
            } else if (strcmp(allergyCounts[i].allergy, "Pollen Allergy") == 0) {
               
                printf("\t1. Keep windows closed during high pollen seasons.\n");
                printf("\t2. Use air conditioning instead of opening windows.\n");
                printf("\t3. Stay indoors on high pollen count days.\n");
                printf("\t4. Shower and change clothes after being outdoors.\n");
                printf("\t5. Use air purifiers with HEPA filters.\n");
                printf("\t6. Follow prescribed allergy medications.\n");
                printf("-------------------------------------------------------------------------------------------------------------\n");
            } else if (strcmp(allergyCounts[i].allergy, "Medicine Allergy") == 0) {
               
                printf("\t1. Keep a list of all medications that trigger allergic reactions.\n");
                printf("\t2. Inform all healthcare providers of the medicine allergy.\n");
                printf("\t3. Wear a medical alert bracelet indicating the allergy.\n");
                printf("\t4. Carry antihistamines or an epinephrine auto-injector.\n");
                printf("\t5. Discuss alternative medications with a healthcare provider.\n");
                printf("-------------------------------------------------------------------------------------------------------------\n");
            }
        }
    }
}

void Reporting_n_Analysis(char *id)
{
    schedulestruct DocSchedule[256];
    ehrstruct ehrinfo[500];
    FILE *ehrtxt = fopen("ehr.txt","r");
    char buffer[500];
    int i = 0;
    int c;

    while (fgets(buffer,500,ehrtxt) != NULL) {
        char *token = strtok(buffer,",");
        strcpy(ehrinfo[i].patientid,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].name,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].gender,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].birthdate,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].insurance,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].allergy,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].medicalhistory,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].medication,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].diagnosticreport,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].appointmenthistory,token);

        token = strtok(NULL,",");
        strcpy(ehrinfo[i].nurseid,token);
        
        i++;
    }

    char patientID[50];
    int reportInput;
    int found = 0;
    allergycount allergyCounts[500];
    int allergyIndex = 0;
    int ehrsize = sizeof(ehrinfo)/sizeof(ehrinfo[0]);
    int nursefound = 0;
    
    while(1)
    {
        printf("\n<<Reporting and Analysis>>\n");
        printf("Do you want to:\n1. Generate Patient Report\n2. Identify Trends\n3. Quit\n");
        printf("Enter your option: ");

        if(scanf("%d", &reportInput) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
        {
            printf("Please enter an integer!\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        
        while ((c = getchar()) != '\n' && c != EOF);
        switch (reportInput)
        {
        case 1:
            for (int i = 0; i < ehrsize; i++)
            {
                if(strcmp(id,ehrinfo[i].nurseid) == 0)
                {
                    nursefound = 1;
                }
            }

            if(nursefound)
            {
                    printf("Your patient: \n");
                    for (int i = 0; i < ehrsize && ehrinfo[i].patientid[0] !='\0'; i++)
                    {
                        if(strcmp(id,ehrinfo[i].nurseid) == 0)
                        {
                            printf("%s,%s\n", ehrinfo[i].patientid,ehrinfo[i].name);
                        }
                    }
            }

            if(!nursefound)
            {
                printf("You do not have any patient yet!\n");
                break;
            }

            printf("\nEnter patient ID: ");
            scanf("%s", patientID);
            patientID[0] = toupper(patientID[0]);

            for(int i = 0; i < sizeof(ehrinfo)/sizeof(ehrinfo[0]); i++)
            {
                if(strcmp(patientID, ehrinfo[i].patientid) == 0 && strcmp(id,ehrinfo[i].nurseid) == 0)
                {
                    printf("Generating patient report...\n");
                    printf("--------------------------------------------------------------\n");
                    printf("\t\t\tPatient Report\t\t\t\n");
                    printf("--------------------------------------------------------------\n");
                    printf("\tPatient ID:           %s\n", ehrinfo[i].patientid);
                    printf("\tName:                 %s\n", ehrinfo[i].name);
                    printf("\tGender:               %s\n", ehrinfo[i].gender);
                    printf("\tBirthDate:            %s\n", ehrinfo[i].birthdate);
                    printf("\tAllergy:              %s\n", ehrinfo[i].allergy);
                    printf("\tMedical History:      %s\n", ehrinfo[i].medicalhistory);
                    printf("\tMedication:           %s\n", ehrinfo[i].medication);
                    printf("\tDiagnostic Report:    %s\n", ehrinfo[i].diagnosticreport);
                    found = 1;
                    break;
                }
            }
            if(!found)
            {
                printf("This patient ID is not in your list!\n");
                break;
            }
            break;
        case 2:
            // Initialize allergyCounts array
            for (int i = 0; i < sizeof(allergyCounts)/ sizeof(allergyCounts[0]); i++) {
                strcpy(allergyCounts[i].allergy, "");
                allergyCounts[i].count = 0;
            }

            // Count allergies across all patients
            for (int i = 0; i < ehrsize; i++) {
                if (strcmp(ehrinfo[i].allergy, "none") != 0 && ehrinfo[i].allergy[0] != '\0') {
                    int found = 0;
                    for (int j = 0; j < allergyIndex; j++) {
                        if (strcmp(allergyCounts[j].allergy, ehrinfo[i].allergy) == 0) {
                            allergyCounts[j].count++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        strcpy(allergyCounts[allergyIndex].allergy, ehrinfo[i].allergy);
                        allergyCounts[allergyIndex].count = 1;
                        allergyIndex++;
                    }
                }
            }

            printAllergyReport(allergyCounts, allergyIndex);
            break;
        case 3:
            printf("Quiting...\n");
            return;
        default:
            printf("Invalid option!\n");
        }
    }
}

int nurse(char *id)
{
    Med_Info medicineInfo[256];
    int size = 0;
    int c;

    // createInventoryFile();
    CopyIntoInventoryArray(medicineInfo, &size);

    while(1)
    {
        printf("\n<<Staff Nurse Menu>>\n1. Schedule\n2. Inventory Management\n3. Reporting and Analysis\n4. Logout\n");
        printf("Enter your option: ");
        int nurseinput;
        
        if(scanf("%d", &nurseinput) != 1) //this code is to prevent user to type alpha(if not will infinite loop)
        {
            printf("Please enter an integer!\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        while ((c = getchar()) != '\n' && c != EOF);
        switch (nurseinput)
        {
        case 1:
            DoctorScheduler(id);
            break;
        case 2:
            InventoryManagement(medicineInfo, &size);
            break;
        case 3:
            Reporting_n_Analysis(id);//discuss with doctor
            break;
        case 4:
            printf("Logging out...\n");
            return 0;
        default:
            printf("Invalid input!\n");
        }
    }
}

#endif
