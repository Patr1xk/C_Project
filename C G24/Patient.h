#ifndef Patient_H
#define Patient_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "Doctor.h"

void patient (char *id)
{ 
    int menu_option;
    char yesorno[20];
    do 
    {
    printf("\n======================================");
    printf("\n|            Patient Menu            |");
    printf("\n======================================");
    printf("\n|                                    |");
    printf("\n|      1. Book Appointment           |");
    printf("\n|      2. View Appointment           |");
    printf("\n|      3. Reschedule Appointment     |");
    printf("\n|      4. Cancel Appointment         |");
    printf("\n|      5. Personal Information       |");
    printf("\n|      6. Billing Details            |");
    printf("\n|                                    |");
    printf("\n======================================\n");
    

    do 
    {
        printf("\nChoose an option: ");
        while (scanf("%d", &menu_option) != 1) // Handle non-integer input
        {      
            printf("\nInvalid Input.\n\nPlease enter a valid menu option: ");
            while(getchar() != '\n'); // Clear input buffer
        }

        if (menu_option < 1 || menu_option > 6)
        {
            printf("\nInvalid Option. Try Again.\n");
        }

    } while (menu_option < 1 || menu_option > 6);

    if (menu_option == 1)
    {
        FILE * userinfoFile;
        bool foundPatient = false;
        char userfile[] = "userdata.txt";
        char userline[1000];
        char delim[] = ",";
        char position[20],fullname[50], gender[20], dob[20], contact[20], username[20], password[20], history[20], insurance[20], room[20];

        schedulestruct schedules[100];
        FILE * doctorSchedule;
        int doctorcount = 0;
        char schedule[] = "schedule.txt";
        char scheduleline[1000];

        int book;
        bool foundAvailable = false;
    
        do
        {
            userinfoFile = fopen(userfile, "r");

            if (userinfoFile == NULL)
            {
                printf("Could not open file %s\n", userfile);
            }

            else
            {
                while (fgets(userline, sizeof(userline), userinfoFile) != NULL) 
                {
                    // Tokenize the line using strtok
                    char *token = strtok(userline, delim);
                    if (token != NULL) 
                    {
                        // Compare the first token with the patient variable
                        if (strcmp(id, token) == 0) 
                        {
                            foundPatient = true;
                            break;
                        }
                    }
                }
            }

            fclose(userinfoFile);

            if (!foundPatient)
            {
                printf("Patient ID Not Found.\n");
            }

        } while (!foundPatient);

        doctorSchedule = fopen(schedule, "r");
        if (doctorSchedule == NULL)
        {
            printf("Could not open file %s\n", schedule);
        }

        else
        {
            while (fgets(scheduleline, sizeof(scheduleline), doctorSchedule)) 
            {
                // Tokenize the line using strtok
                char *token = strtok(scheduleline, delim);
                int tokenIndex = 0; // Track token index for each field
                while (token != NULL) 
                {
                    switch(tokenIndex) 
                    {
                        case 0: 
                            strcpy(schedules[doctorcount].doctorid,token); 
                            break;
                        case 1: 
                            strcpy(schedules[doctorcount].name,token); 
                            break;
                        case 2: 
                            strcpy(schedules[doctorcount].day,token); 
                            break;
                        case 3: 
                            strcpy(schedules[doctorcount].timeslot,token); 
                            break;
                        case 4: 
                            strcpy(schedules[doctorcount].availability,token);
                            schedules[doctorcount].availability[strcspn(schedules[doctorcount].availability, "\n")] = '\0';
                            break;

                        case 5:
                            strcpy(schedules[doctorcount].nurseid,token);
                            schedules[doctorcount].nurseid[strcspn(schedules[doctorcount].nurseid, "\n")] = '\0';
                            break;
                        }
                        token = strtok(NULL, delim); // Get the next token
                        tokenIndex++; // Move to the next field
                }

                doctorcount++;

                // Print schedules
                printf("\nNo. %d\n", doctorcount);
                printf("Doctor ID    : %s\n", schedules[doctorcount-1].doctorid);
                printf("Doctor Name  : %s\n", schedules[doctorcount-1].name);
                printf("Day          : %s\n", schedules[doctorcount-1].day);
                printf("Time Slot    : %s\n", schedules[doctorcount-1].timeslot);
                printf("Nurse ID     : %s\n", schedules[doctorcount-1].nurseid);

                if (strcmp(schedules[doctorcount-1].availability, "Y") == 0)
                {
                    printf("Availability : Available\n");
                }
                else
                {
                    printf("Availability : Unavailable\n");
                }
               
            }
            fclose(doctorSchedule);
        }

        do 
        {
            printf("\nEnter Number of Schedule to Book: ");

            while (scanf("%d", &book) != 1) // Handle non-integer input
            {      
                printf("\nInvalid Input.\n\nPlease enter a valid number: ");
                while(getchar() != '\n'); // Clear input buffer
            }

            if (book >= 1 && book <= doctorcount) 
            {
                foundAvailable = true;
                if (strcmp(schedules[book - 1].availability, "Y") != 0)
                {
                    foundAvailable = false;
                    printf("The selected schedule is not available for booking.\n");
                } 

                else
                {
                    printf("\nBooking Successfully!\n");

                    strcpy(schedules[book-1].availability, id);

                    doctorSchedule = fopen(schedule, "w");

                    for (int i = 0; i < doctorcount; i++) 
                    {
                        fprintf(doctorSchedule, "%s,%s,%s,%s,%s,%s,\n",
                                schedules[i].doctorid,
                                schedules[i].name,
                                schedules[i].day,
                                schedules[i].timeslot,
                                schedules[i].availability,
                                schedules[i].nurseid);
                    }

                    fclose(doctorSchedule);
                }
            } 
        
            else 
            {
                printf("Invalid schedule number.\n");
            }
        } while (!foundAvailable);  
    }
    

    else if (menu_option == 2)
    {
        schedulestruct schedules[100];
        FILE * doctorSchedule;
        char schedule[] = "schedule.txt"; 
        char scheduleline[1000];
        int appointmentcount = 0;
        bool foundPID = false;
        char delim[] = ",";

        do 
        {
            doctorSchedule = fopen(schedule, "r");
            if (doctorSchedule == NULL)
            {
                printf("Could not open file %s\n", schedule);
            }

            else 
            {
                while (fgets(scheduleline, sizeof(scheduleline), doctorSchedule)) 
                {
                    char *token = strtok(scheduleline, delim);

                    int tokenIndex = 0;
                    schedulestruct tempSchedule;

                    while (token != NULL)
                    {
                        switch (tokenIndex) 
                        {
                            case 0:
                                strcpy(tempSchedule.doctorid, token);
                                break;
                            case 1:
                                strcpy(tempSchedule.name, token);
                                break;
                            case 2:
                                strcpy(tempSchedule.day, token);
                                break;
                            case 3:
                                strcpy(tempSchedule.timeslot, token);
                                break;
                            case 4:
                                strcpy(tempSchedule.availability, token);
                                break;
                            case 5:
                                strcpy(tempSchedule.nurseid, token);
                                tempSchedule.nurseid[strcspn(tempSchedule.nurseid, "\n")] = '\0';  // Remove newline character
                                break;
                        }
                        token = strtok(NULL, delim);
                        tokenIndex++;
                    }
                    
                    if (strcmp(tempSchedule.availability, id) == 0) 
                    {
                        foundPID = true;
                        schedules[appointmentcount++] = tempSchedule;
                    }
                }
                
            }
            fclose(doctorSchedule);

            if(!foundPID)
            {
                printf("No Appointment Found.\n");
                continue;
            }

            for (int i = 0; i < appointmentcount; i++) 
            {
                printf("\nAppointment %d\n", i + 1);
                printf("Doctor ID    : %s\n", schedules[i].doctorid);
                printf("Doctor Name  : %s\n", schedules[i].name);
                printf("Day          : %s\n", schedules[i].day);
                printf("Time Slot    : %s\n", schedules[i].timeslot);
                printf("Nurse ID     : %s\n", schedules[i].nurseid);
            }
            
        } while (!foundPID);
    }


    else if (menu_option == 3)
    {
        FILE *scheduletxt = fopen("schedule.txt","r");

        char buffer[255];
        schedulestruct schedulelist[255];
        int i = 0;

        // Reads from file and store in struct
        while (fgets(buffer,255,scheduletxt) != NULL) { 
            char *token = strtok(buffer,",");
            strcpy(schedulelist[i].doctorid,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].name,token);
            
            token = strtok(NULL,",");
            strcpy(schedulelist[i].day,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].timeslot,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].availability,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].nurseid,token);

            i++;
        }

        int appointmentcount = 0;
        int input;
        int input2;

        for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) {
            if (strcmp(schedulelist[i].availability,id) == 0) {
                printf("Slot        :%-20d\n",appointmentcount + 1);
                printf("Doctor ID   :%-20s\n",schedulelist[i].doctorid);
                printf("Name        :%-20s\n",schedulelist[i].name);
                printf("Day         :%-20s\n",schedulelist[i].day);
                printf("Timeslot    :%-20s\n\n",schedulelist[i].timeslot);

                appointmentcount++;
            } 
        }

        do 
        {
            printf("Enter the slot of the appointment that you want to reschedule: ");
            scanf("%d",&input);

            if (!(input > 0 && input <= appointmentcount)) 
            {   
                while ((getchar()) != '\n');
                printf("Enter a valid number\n");
            }
            else 
            {
                for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) {
                    if (schedulelist[i].availability[0] == 'P') {
                        printf("Slot        : %-20d\n",i + 1);
                        printf("Doctor ID   : %-20s\n",schedulelist[i].doctorid);
                        printf("Name        : %-20s\n",schedulelist[i].name);
                        printf("Day         : %-20s\n",schedulelist[i].day);
                        printf("Timeslot    : %-20s\n",schedulelist[i].timeslot);
                        printf("Availability: Unavailable\n\n");
                    } 
                    else {
                        printf("Slot        : %-20d\n",i + 1);
                        printf("Doctor ID   : %-20s\n",schedulelist[i].doctorid);
                        printf("Name        : %-20s\n",schedulelist[i].name);
                        printf("Day         : %-20s\n",schedulelist[i].day);
                        printf("Timeslot    : %-20s\n",schedulelist[i].timeslot);
                        printf("Availability: Available\n\n");
                    }
                }

                do {
                    printf("Which slot do you want to reschedule to: ");
                    scanf("%d",&input2);

                    if (schedulelist[input2 - 1].availability[0] == 'P') {
                        while ((getchar()) != '\n');
                        printf("The slot is unavailable\n");
                    }
                    else {
                        scheduletxt = fopen("schedule.txt","w");
                        fprintf(scheduletxt,"");
                        scheduletxt = fopen("schedule.txt","a");

                        int counter = 1;

                        for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) {
                            if (i == (input2 - 1)) {
                                fprintf(scheduletxt,"%s,%s,%s,%s,%s,%s,\n",
                                        schedulelist[i].doctorid,
                                        schedulelist[i].name,
                                        schedulelist[i].day,
                                        schedulelist[i].timeslot,
                                        id,
                                        schedulelist[i].nurseid);
                            }
                            else if (strcmp(schedulelist[i].availability,id) == 0 && input == counter) {
                                fprintf(scheduletxt,"%s,%s,%s,%s,Y,%s,\n",
                                        schedulelist[i].doctorid,
                                        schedulelist[i].name,
                                        schedulelist[i].day,
                                        schedulelist[i].timeslot,
                                        schedulelist[i].nurseid);
                                        counter = -999999;
                            }
                            else if (strcmp(schedulelist[i].availability,id) == 0) {
                                counter++;
                                fprintf(scheduletxt,"%s,%s,%s,%s,%s,%s,\n",
                                        schedulelist[i].doctorid,
                                        schedulelist[i].name,
                                        schedulelist[i].day,
                                        schedulelist[i].timeslot,
                                        schedulelist[i].availability,
                                        schedulelist[i].nurseid);
                            }
                            else {
                                fprintf(scheduletxt,"%s,%s,%s,%s,%s,%s,\n",
                                        schedulelist[i].doctorid,
                                        schedulelist[i].name,
                                        schedulelist[i].day,
                                        schedulelist[i].timeslot,
                                        schedulelist[i].availability,
                                        schedulelist[i].nurseid);
                            }
                        }
                        fclose(scheduletxt);
                    }
                }
                while (schedulelist[input2 - 1].availability[0] == 'P');
            }
        } 
        while (!(input > 0 && input <= appointmentcount));
    }
 
    else if (menu_option == 4)
    {
        FILE *scheduletxt = fopen("schedule.txt","r");

        char buffer[255];
        schedulestruct schedulelist[255];
        int i = 0;

        // Reads from file and store in struct
        while (fgets(buffer,255,scheduletxt) != NULL) { 
            char *token = strtok(buffer,",");
            strcpy(schedulelist[i].doctorid,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].name,token);
            
            token = strtok(NULL,",");
            strcpy(schedulelist[i].day,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].timeslot,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].availability,token);

            token = strtok(NULL,",");
            strcpy(schedulelist[i].nurseid,token);

            i++;
        }

        int appointmentcount = 0;
        int input;

        for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) {
            if (strcmp(schedulelist[i].availability,id) == 0) {
                printf("Slot        :%-20d\n",appointmentcount + 1);
                printf("Doctor ID   :%-20s\n",schedulelist[i].doctorid);
                printf("Name        :%-20s\n",schedulelist[i].name);
                printf("Day         :%-20s\n",schedulelist[i].day);
                printf("Timeslot    :%-20s\n\n",schedulelist[i].timeslot);

                appointmentcount++;
            } 
        }

        printf("You have %d appointments\n\n",appointmentcount);

        do 
        {
            printf("Enter the slot of the appointment that you want to cancel: ");
            scanf("%d",&input);

            if (!(input > 0 && input <= appointmentcount)) 
            {   
                while ((getchar()) != '\n');
                printf("Enter a valid number\n");
            }
            else 
            {
                scheduletxt = fopen("schedule.txt","w");
                fprintf(scheduletxt,"");
                scheduletxt = fopen("schedule.txt","a");
                
                int counter = 1;

                for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) 
                {   
                    if (strcmp(schedulelist[i].availability,id) == 0 && input == counter) 
                    {
                        fprintf(scheduletxt,"%s,%s,%s,%s,Y,%s,\n",
                            schedulelist[i].doctorid,
                            schedulelist[i].name,
                            schedulelist[i].day,
                            schedulelist[i].timeslot,
                            schedulelist[i].nurseid);
                        counter = -999999; //scuffed
                    }
                    else if (strcmp(schedulelist[i].availability,id) == 0)
                    {
                        counter++;
                        fprintf(scheduletxt,"%s,%s,%s,%s,%s,%s,\n",
                            schedulelist[i].doctorid,
                            schedulelist[i].name,
                            schedulelist[i].day,
                            schedulelist[i].timeslot,
                            schedulelist[i].availability,
                            schedulelist[i].nurseid);
                    }
                    else 
                    {
                        fprintf(scheduletxt,"%s,%s,%s,%s,%s,%s,\n",
                            schedulelist[i].doctorid,
                            schedulelist[i].name,
                            schedulelist[i].day,
                            schedulelist[i].timeslot,
                            schedulelist[i].availability,
                            schedulelist[i].nurseid);
                    } 
                }
                printf("Appointment Successfully Cancelled\n");
                fclose(scheduletxt);
            }
        } 
        while (!(input > 0 && input <= appointmentcount));        
    }

    else if (menu_option == 5)
    {
        FILE *ehrtxt;
        char ehrfile[] = "ehr.txt";
        char ehrline[1000];
        ehrstruct ehrlist[1000];
        bool foundPatient = false;
        char delim[] = ",";
        int ehrCount = 0;

        do
        {
            ehrtxt = fopen(ehrfile, "r");

            if (ehrtxt == NULL)
            {
                printf("Could not open file %s\n", ehrfile);
                continue;
            }

            else
            {
                while (fgets(ehrline, sizeof(ehrline), ehrtxt)) 
                {
                    char *token = strtok(ehrline, delim);
                    if (token != NULL && strcmp(id, token) == 0) 
                    {
                        foundPatient = true;

                        strcpy(ehrlist[ehrCount].patientid, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].name, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].gender, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].birthdate, token);

                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].insurance, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].allergy, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].medicalhistory, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].medication, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].diagnosticreport, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].appointmenthistory, token);
                        
                        // token = strtok(NULL, delim);
                        // strcpy(ehrlist[ehrCount].billinginformation, token);

                        token = strtok(NULL, delim);

                        ehrCount++;

                        if (ehrCount >= 1)
                        break;  // Prevent overflow
                    }
                }
            }

            fclose(ehrtxt);

            if (!foundPatient)
            {
                printf("Patient ID Not Found.\n");
                continue;
            }

            for (int i = 0 ; i < ehrCount ; i++) 
            {
                printf("\nPatient ID          : %s\n",ehrlist[i].patientid);
                printf("Name                : %s\n",ehrlist[i].name);
                printf("Gender              : %s\n",ehrlist[i].gender);
                printf("Birthdate           : %s",ehrlist[i].birthdate);

                if (strcmp(ehrlist[i].insurance,"Y")==0)
                {
                    printf("\nInsurance           : Yes\n");
                }

                else
                {
                    printf("\nInsurance           : none\n");
                }

                printf("Allergy             : %s\n",ehrlist[i].allergy);
                printf("Medical History     : %s\n",ehrlist[i].medicalhistory);
                printf("Diagnostic Report   : %s\n",ehrlist[i].diagnosticreport);
            }

        } while (!foundPatient);     
    }


    else if (menu_option == 6)
    {
        FILE *ehrtxt;
        char ehrfile[] = "ehr.txt";
        char ehrline[1000];
        ehrstruct ehrlist[1000];
        bool foundPatient = false;
        char delim[] = ",";
        int ehrCount = 0;

        FILE *inventorytxt;
        char inventoryfile[] = "inventory.txt";
        char inventoryline[1000];
        inventorystruct inventorylist[1000];
        int inventoryCount = 0;

        int billingCount = 1;

        do
        {
            ehrtxt = fopen(ehrfile, "r");

            if (ehrtxt == NULL)
            {
                printf("Could not open file %s\n", ehrfile);
                continue;
            }

            else
            {
                while (fgets(ehrline, sizeof(ehrline), ehrtxt)) 
                {
                    char *token = strtok(ehrline, delim);
                    if (token != NULL && strcmp(id, token) == 0) 
                    {
                        foundPatient = true;

                        strcpy(ehrlist[ehrCount].patientid, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].name, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].gender, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].birthdate, token);

                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].insurance, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].allergy, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].medicalhistory, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].medication, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].diagnosticreport, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(ehrlist[ehrCount].appointmenthistory, token);
                        
                        // token = strtok(NULL, delim);
                        // strcpy(ehrlist[ehrCount].billinginformation, token);

                        ehrCount++;

                        if (ehrCount >= sizeof(ehrlist)/sizeof(ehrlist[0])) 
                        break;  // Prevent overflow
                    }
                }
            }

            fclose(ehrtxt);

            if (!foundPatient)
            {
                printf("Patient ID Not Found.\n");
                continue;
            }

            inventorytxt = fopen(inventoryfile, "r");

            if (inventorytxt == NULL)
            {
                printf("Could not open file %s\n", inventoryfile);
                continue;
            }

            else
            {
                while (fgets(inventoryline, sizeof(inventoryline), inventorytxt)) 
                {
                    char *token = strtok(inventoryline, delim);
                    if (token != NULL) 
                    {
                        foundPatient = true;

                        strcpy(inventorylist[inventoryCount].medicineid, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(inventorylist[inventoryCount].name, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(inventorylist[inventoryCount].indication, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(inventorylist[inventoryCount].quantity, token);

                        token = strtok(NULL, delim);
                        strcpy(inventorylist[inventoryCount].price, token);
                        
                        token = strtok(NULL, delim);
                        strcpy(inventorylist[inventoryCount].expdate, token);

                        token = strtok(NULL, delim);
                        strcpy(inventorylist[inventoryCount].alert, token);

                        inventoryCount++;

                        if (inventoryCount >= sizeof(inventorylist)/sizeof(inventorylist[0])) 
                        break;  // Prevent overflow
                    }
                }
            }

            fclose(inventorytxt);

            if (!foundPatient)
            {
                printf("Patient ID Not Found.\n");
                continue;
            }

            for (int i = 0 ; i < ehrCount ; i++) 
            {
                printf("\n%d. Billing Information\n",billingCount);

                for (int j = 0 ; j < inventoryCount ; j++)
                {
                    if (strcmp(ehrlist[i].medication, inventorylist[j].name) == 0)
                    {
                        printf("Medicine ID   : %s\n",inventorylist[j].medicineid);
                        printf("Medicine Name : %s\n",inventorylist[j].name);
                        printf("Usage         : %s\n",inventorylist[j].indication);
                        printf("Expiry Date   : %s\n",inventorylist[j].expdate);
                        printf("Remarks       : %s\n",inventorylist[j].alert);

                        float price = atof(inventorylist[j].price);

                        if (strcmp(ehrlist[i].insurance, "Y") == 0)
                        {
                            float total = price * 0.95;
                            printf("Price         : RM %.2f (Insurance Claimed)\n",total);
                            break;
                        }

                        else
                        {
                            printf("Price         : RM %s\n",inventorylist[j].price);
                            break;
                        }
                    }
                }
                billingCount ++;
            }

        } while (!foundPatient);     
    }
    
    do
    {
        printf("\nDo you want to continue? (Y/N): ");
        scanf("%s", yesorno);
        yesorno[0] = toupper(yesorno[0]);

        if(yesorno[0] != 'Y' && yesorno[0] != 'N')
        {
            printf("Invalid Input.\n");
        }

        else if(strlen(yesorno) > 1)
        {
            printf("Type Only 'Y' or 'N'\n");
        }

    } while (strlen(yesorno) > 1 || yesorno[0] != 'Y' && yesorno[0] != 'N');

} while (strlen(yesorno) < 1 || yesorno[0] != 'N' );

}

#endif