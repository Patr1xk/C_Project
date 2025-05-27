#ifndef doctor_h
#define doctor_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *strupr(char *str) {
    char *orig = str;
    while (*str != '\0') {
        *str = toupper((unsigned char)*str);
        str++;
    }
    return orig;
}

typedef struct {
    char doctorid[20];
    char name[20];
    char day[20];
    char timeslot[20];
    char availability[20]; //PatientID indicates its been booked, otherwise 'Y'
    char nurseid[20];
} schedulestruct;

typedef struct {
    char nurseid[20];
    char name[20];
} nursestruct;

typedef struct {
    char patientid[50];
    char name[50];
    char gender[50];
    char birthdate[50];
    char insurance[50];
    char allergy[50];
    char medicalhistory[50]; // = past procedure
    char medication[50]; // = prescriptions
    char diagnosticreport[50];
    char appointmenthistory[50];
    char nurseid[50];
} ehrstruct;

//Id,name,indication,quantity,price,exp date,alert
typedef struct {
    char medicineid[20];
    char name[20];
    char indication[20];
    char quantity[20];
    char price[20];
    char expdate[20];
    char alert[20];
} inventorystruct;

void DoctorScheduleManager(char *id) {
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

    FILE *userdatatxt = fopen("userdata.txt","r");

    char name[20];
    nursestruct nurselist[255];
    i = 0;

    // Reads from file and store in struct
    while (fgets(buffer,255,userdatatxt) != NULL) {
        char *token = strtok(buffer,",");

        if ((strcmp(token,id)) == 0) {
            token = strtok(NULL,",");
            strcpy(name,token);
        }
        else if (token[0] == 'N') {
            strcpy(nurselist[i].nurseid,token);

            token = strtok(NULL,",");
            strcpy(nurselist[i].name,token);
            
            i++;
        }        
    }

    scheduletxt = fopen("schedule.txt","a");

    int size = sizeof(schedulelist)/sizeof(schedulelist[0]);
    int input_number;

    char day[7][10] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
    char time[20];
    char nurseid[20];

    // Loops 3 attempts
    for (int i = 0; i < 3; i++) {
        char valid = 'Y';

        printf("1.Add Schedule\n2.Cancel Schedule\nEnter the number corresponding to the action:");
        scanf("%d",&input_number);
        
        // Add schedule
        if (input_number == 1) {
            printf("Enter the starting time of the appointment slot:");
            scanf("%d",&input_number);

            if (input_number > 0 && input_number < 6) {
                sprintf(time,"%dpm-%dpm",input_number,input_number+1);
                printf("Duration of the appointment slot: %s\n",time);
            }
            else if (input_number > 7 && input_number < 11) {
                sprintf(time,"%dam-%dam",input_number,input_number+1);
                printf("Duration of the appointment slot: %s\n",time);
            }
            else if (input_number == 11) {
                sprintf(time,"11am-12pm");
                printf("Duration of the appointment slot: %s\n",time);
            }
            else if (input_number == 12) {
                sprintf(time,"12pm-1pm");
                printf("Duration of the appointment slot: %s\n",time);
            }
            else {printf("Invalid Number\n"); valid = 'N'; while ((getchar()) != '\n');} // Clears input buffer if invalid input

            // Check if time is replaced with string, if not skip
            if (strlen(time) > 0) {
                printf("1.Monday\n2.Tuesday\n3.Wednesday\n4.Thursday\n5.Friday\n6.Saturday\n7.Sunday\nWhen is the appointment day:");
                scanf("%d",&input_number);

                if (input_number > 0 && input_number < 8) {
                    for (int i = 0; i < size && schedulelist[i].name[0] != '\0'; i++) {
                        // Check if name with same day and time exists to prevent duplicates
                        if (strcmp(schedulelist[i].name,name) == 0 && strcmp(schedulelist[i].day,day[input_number-1]) == 0 && strcmp(schedulelist[i].timeslot,time) == 0) {
                            printf("This timeslot already exists\n");
                            valid = 'N';
                        }    
                    }
                }
                else {printf("Invalid Number\n"); valid = 'N'; while ((getchar()) != '\n');} // Clears input buffer if invalid input        
            }

            if (valid == 'Y') {
                valid = 'N';
                for (int i = 0; i < sizeof(nurselist)/sizeof(nurselist[0]) && nurselist[i].name[0] != '\0'; i++) {
                    printf("Nurse ID: %s    Name: %s\n",nurselist[i].nurseid,nurselist[i].name);
                }

                printf("Who would you like to assign to the timeslot:");
                scanf("%s",nurseid);
                strupr(nurseid);

                for (int i = 0; i < sizeof(nurselist)/sizeof(nurselist[0]) && nurselist[i].name[0] != '\0'; i++) {
                    if (strcmp(nurseid,nurselist[i].nurseid) == 0) {
                        valid = 'Y';
                        break;
                    }
                }

                if (valid == 'Y') {
                    fprintf(scheduletxt,"%s,%s,%s,%s,Y,%s,\n",id,name,day[input_number-1],time,nurseid);
                    printf("Schedule Added\n");
                    i = 3;
                }
                else {printf("Invalid Nurse ID\n"); while ((getchar()) != '\n');} // Clears input buffer if invalid input
            }
        }
        // Delete schedule
        else if (input_number == 2) {
            int count = 0;

            for (int i = 0; i < size && schedulelist[i].name[0] != '\0'; i++) {
                printf("===================================\n");
                printf("|Slot        :%-20d|\n",i+1);
                printf("|Doctor ID   :%-20s|\n",schedulelist[i].doctorid);
                printf("|Name        :%-20s|\n",schedulelist[i].name);
                printf("|Day         :%-20s|\n",schedulelist[i].day);
                printf("|Timeslot    :%-20s|\n",schedulelist[i].timeslot);
                printf("|Availability:%-20s|\n",schedulelist[i].availability);
                printf("|Nurse ID    :%-20s|\n",schedulelist[i].nurseid);
                count++;
            }
            printf("===================================\n");

            printf("%d\n",count);
            printf("Enter the slot number to cancel:");
            scanf("%d",&input_number);

            if (input_number - 1 < count && strcmp(id,schedulelist[input_number - 1].doctorid) == 0) {
                scheduletxt = fopen("schedule.txt","w");
                fprintf(scheduletxt,"");
                scheduletxt = fopen("schedule.txt","a");

                for (int i = 0; i < size && schedulelist[i].name[0] != '\0'; i++) {
                    if (i != input_number - 1) {
                        fprintf(scheduletxt,"%s,%s,%s,%s,%s,%s,\n",schedulelist[i].doctorid,schedulelist[i].name,schedulelist[i].day,schedulelist[i].timeslot,schedulelist[i].availability,schedulelist[i].nurseid);
                    }
                }

                i = 3;
                printf("Schedule Cancelled\n");
            }
            else {printf("Invalid Number or You do not have permission to modify this slot\n"); valid = 'N'; while ((getchar()) != '\n');}
        }
        
        if (i > 1 && valid == 'N') {
            printf("You have exceeded the number of attempts\n");
        }
    }
    
    fclose(scheduletxt); fclose(userdatatxt);
}

void ElectronicHealthRecords() {
    FILE *ehrtxt = fopen("ehr.txt","r");

    char buffer[600];
    ehrstruct ehrlist[600];
    int i = 0;

    // read ehrtxt stores in struct
    while (fgets(buffer,600,ehrtxt) != NULL) {
        char *token = strtok(buffer,",");
        strcpy(ehrlist[i].patientid,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].name,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].gender,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].birthdate,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].insurance,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].allergy,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].medicalhistory,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].medication,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].diagnosticreport,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].appointmenthistory,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].nurseid,token);

        i++;
    }

    FILE *inventorytxt = fopen("Inventory.txt","r");

    inventorystruct inventorylist[600];
    i = 0;

    //reads inventory txt stores in struct
    //Id,brand,name,indication,quantity,price,exp date,alert
    while (fgets(buffer,600,inventorytxt) != NULL) {
        char *token = strtok(buffer,",");
        strcpy(inventorylist[i].medicineid,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].name,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].indication,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].quantity,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].price,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].expdate,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].alert,token);

        i++;
    }

    int patientindex;
    int input_number;
    char patientid[20];
    char input_string[20];
    char valid = 'N';
    int temperature;
    int bloodpressure;
    int bloodpressure2;
    char medhistory[50];
    FILE *scheduletxt = fopen("schedule.txt","r");
    //char buffer[255];
    schedulestruct schedulelist[255];
    //int i = 0;

    char allergy[4][20] = {"Food Allergy","Pollen Allergy","Medication Allergy","Animal Allergy"};

    printf("1. View EHR\n2. Update EHR\nWhat would you like to do: ");
    scanf("%d",&input_number);

    if (input_number == 1) {
        printf("\n");
        for (int i = 0; i < sizeof(ehrlist)/sizeof(ehrlist[0]) && ehrlist[i].patientid[0] != '\0'; i++) {
            printf("Patient ID          : %s\n",ehrlist[i].patientid);
            printf("Name                : %s\n",ehrlist[i].name);
            printf("Gender              : %s\n",ehrlist[i].gender);
            printf("Birthdate           : %s\n",ehrlist[i].birthdate);
            printf("Insurance           : %s\n",ehrlist[i].insurance);
            printf("Allergy             : %s\n",ehrlist[i].allergy);
            printf("Medical History     : %s\n",ehrlist[i].medicalhistory);
            printf("Medication          : %s\n",ehrlist[i].medication);
            
            if (strlen(ehrlist[i].diagnosticreport) > 5) {
                char *token = strtok(ehrlist[i].diagnosticreport," ");
                printf("Body Temperature °C : %s\n",token);

                token = strtok(NULL," ");
                printf("Blood Pressure      : %s\n",token);

                token = strtok(NULL,",");
                printf("Pulse Rate          : %s\n",token);
            }
            else {
                printf("Body Temperature °C : -\n");
                printf("Blood Pressure      : -\n");
                printf("Pulse Rate          : -\n");
            }

            printf("Appointment History : %s\n\n",ehrlist[i].appointmenthistory);
        }
    }  
    else if (input_number == 2) {
        for (int i = 0; i < sizeof(ehrlist)/sizeof(ehrlist[0]) && ehrlist[i].patientid[0] != '\0'; i++) {
            printf("Patient ID          : %s\n",ehrlist[i].patientid);
            printf("Name                : %s\n",ehrlist[i].name);
            printf("Gender              : %s\n",ehrlist[i].gender);
            printf("Birthdate           : %s\n\n",ehrlist[i].birthdate);
        }
        printf("Enter the ID of the patient: ");
        scanf("%s",patientid);
        strupr(patientid);

        for (int i = 0; i < sizeof(ehrlist)/sizeof(ehrlist[0]) && ehrlist[i].patientid[0] != '\0'; i++) {
            if (strcmp(ehrlist[i].patientid,patientid) == 0 ) {
                valid = 'Y';
                patientindex = i;
                break;
            }
        }

        if (valid == 'Y') {
            valid = 'N';

            printf("1. Allergy\n2. Medication\n3. Diagnostic Report\n4. Appointment History\n5. Medical History\nWhat would you like to update: ");
            scanf("%d",&input_number);

            switch (input_number) {
                case 1:
                    printf("1. Food Allergy\n2. Pollen Allergy\n3. Medication Allergy\n4. Animal Allergy\nWhat type of allergy does the patient have: ");
                    scanf("%d",&input_number);

                    if (input_number > 0 && input_number < 5) {
                        strcpy(ehrlist[patientindex].allergy,allergy[input_number - 1]);
                        valid = 'Y';
                    } else {printf("Invalid Number\n"); valid = 'N'; while ((getchar()) != '\n');} // Clears input buffer if invalid input
        
                    break;


                case 2:
                    for (int i = 0; i < sizeof(inventorylist)/sizeof(inventorylist[0]) && inventorylist[i].medicineid[0] != '\0'; i++) {
                        if (inventorylist[i].medicineid[0] == 'M') {
                            printf("Medicine ID : %s\n",inventorylist[i].medicineid);
                            printf("Name        : %s\n",inventorylist[i].name);
                            printf("Use         : %s\n",inventorylist[i].indication);
                            printf("Quantity    : %s\n\n",inventorylist[i].quantity);
                        }
                    }

                    printf("Enter the medicine ID: ");
                    scanf("%s",input_string);
                    strupr(input_string);

                    for (int i = 0; i < sizeof(inventorylist)/sizeof(inventorylist[0]) && inventorylist[i].medicineid[0] != '\0'; i++) {
                        if (strcmp(inventorylist[i].medicineid,input_string) == 0) {
                            strcpy(ehrlist[patientindex].medication,inventorylist[i].name);

                            int placeholder = atoi(inventorylist[i].quantity) - 1;
                            sprintf(inventorylist[i].quantity,"%d",placeholder);
                        
                            valid = 'Y';
                            break;
                        }
                    }

                    if (valid == 'Y') {
                        inventorytxt = fopen("Inventory.txt","w");
                        fprintf(inventorytxt,"");
                        inventorytxt = fopen("Inventory.txt","a");

                        for (int i = 0; i < sizeof(inventorylist)/sizeof(inventorylist[0]) && inventorylist[i].medicineid[0] != '\0'; i++) {
                            fprintf(inventorytxt,"%s,%s,%s,%s,%s,%s,%s,\n",inventorylist[i].medicineid,inventorylist[i].name,inventorylist[i].indication,inventorylist[i].quantity,inventorylist[i].price,inventorylist[i].expdate,inventorylist[i].alert);
                        }
                    } else {printf("Invalid Number\n"); valid = 'N'; while ((getchar()) != '\n');} // Clears input buffer if invalid input

                    break;


                case 3:                
                    // int temperature;
                    // int bloodpressure;
                    // int bloodpressure2;

                    printf("What is the temperature of the patient: ");
                    scanf("%d",&temperature);

                    if (temperature > 21 && temperature < 44) {
                        printf("What is the Systolic blood pressure of the patient: ");
                        scanf("%d",&bloodpressure);

                        if (bloodpressure > 89 && bloodpressure < 181) {
                            printf("What is the Diastolic blood pressure of the patient: ");
                            scanf("%d",&bloodpressure2);

                            if (bloodpressure2 > 59 && bloodpressure2 < 121) {
                                printf("What is the pulse rate of the patient: ");
                                scanf("%d",&input_number);
                                
                                if (input_number > 59 && input_number < 101) {
                                    valid = 'Y';
                                    sprintf(ehrlist[patientindex].diagnosticreport,"%d %d/%d %d",temperature,bloodpressure,bloodpressure2,input_number);
                                }
                                else {printf("The patient is most likely dead\n\n"); valid = 'N'; while ((getchar()) != '\n');}
                            }
                            else {printf("The patient is most likely dead\n\n"); valid = 'N'; while ((getchar()) != '\n');}
                        }
                        else {printf("The patient is most likely dead\n\n"); valid = 'N'; while ((getchar()) != '\n');}
                    }
                    else {printf("The patient is most likely dead\n\n"); valid = 'N'; while ((getchar()) != '\n');}

                    break;


                case 4:
                    // FILE *scheduletxt = fopen("schedule.txt","r");

                    // char buffer[255];
                    // schedulestruct schedulelist[255];
                    // int i = 0;

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

                    for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) {
                        if (strcmp(patientid,schedulelist[i].availability) == 0) {
                            valid = 'Y';
                            char placeholder[50];
                            strcpy(placeholder,schedulelist[i].timeslot);
                            for (int i = 0; i < sizeof(ehrlist)/sizeof(ehrlist[0]) && ehrlist[i].patientid[0] != '\0'; i++) {
                                if (strcmp(patientid,ehrlist[i].patientid) == 0) {
                                    sprintf(ehrlist[i].appointmenthistory,"%s",placeholder);
                                }                                   
                            }
                        }
                    }

                    break;


                case 5:
                    // char medhistory[50];
                    valid = 'Y';

                    while ((getchar()) != '\n');
                    printf("What type of medical history does patient have: ");
                    fgets(medhistory,50,stdin);
                    medhistory[strlen(medhistory) - 1] = '\0';

                    strcpy(ehrlist[patientindex].medicalhistory,medhistory);

                    break;


                default:
                    printf("Invalid Number\n"); valid = 'N'; while ((getchar()) != '\n');
            }
        }     
    }
    else {printf("Invalid Number\n"); valid = 'N'; while ((getchar()) != '\n');} // Clears input buffer if invalid input

    if (valid == 'Y') {
        ehrtxt = fopen("ehr.txt","w");
        fprintf(ehrtxt,"");
        ehrtxt = fopen("ehr.txt","a");

        for (int i = 0; i < sizeof(ehrlist)/sizeof(ehrlist[0]) && ehrlist[i].patientid[0] != '\0'; i++) {
            fprintf(ehrtxt,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,\n",ehrlist[i].patientid,ehrlist[i].name,ehrlist[i].gender,ehrlist[i].birthdate,ehrlist[i].insurance,ehrlist[i].allergy,ehrlist[i].medicalhistory,ehrlist[i].medication,ehrlist[i].diagnosticreport,ehrlist[i].appointmenthistory,ehrlist[i].nurseid);
        }
        printf("Update Successfully\n");
    }

    fclose(ehrtxt); fclose(inventorytxt);
}

void Report(char *id) {
    FILE *ehrtxt = fopen("ehr.txt","r");

    char buffer[600];
    ehrstruct ehrlist[600];
    int i = 0;

    // read ehrtxt stores in struct
    while (fgets(buffer,600,ehrtxt) != NULL) {
        char *token = strtok(buffer,",");
        strcpy(ehrlist[i].patientid,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].name,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].gender,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].birthdate,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].insurance,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].allergy,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].medicalhistory,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].medication,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].diagnosticreport,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].appointmenthistory,token);

        token = strtok(NULL,",");
        strcpy(ehrlist[i].nurseid,token);

        i++;
    }

    FILE *inventorytxt = fopen("Inventory.txt","r");

    inventorystruct inventorylist[600];
    i = 0;

    //reads inventory txt stores in struct
    //Id,brand,name,indication,quantity,price,exp date,alert
    while (fgets(buffer,600,inventorytxt) != NULL) {
        char *token = strtok(buffer,",");
        strcpy(inventorylist[i].medicineid,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].name,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].indication,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].quantity,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].price,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].expdate,token);

        token = strtok(NULL,",");
        strcpy(inventorylist[i].alert,token);

        i++;
    }

    FILE *scheduletxt = fopen("schedule.txt","r");

    schedulestruct schedulelist[600];
    i = 0;

    while (fgets(buffer,600,scheduletxt) != NULL) { 
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
    
    int count = 0;
    
    printf("\n----------------------------------------------------------------------------\n");
    printf("|                        Reporting & Diagnostics                           |\n");
    printf("----------------------------------------------------------------------------\n");
    printf("| Doctor ID           : %-50s |\n",id);

    for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) {
        if (strcmp(id,schedulelist[i].doctorid) == 0) {
            printf("| Doctor Name         : %-50s |\n",schedulelist[i].name);
            break;
        }
    }

    printf("----------------------------------------------------------------------------\n");
    printf("|                           Patients Treated                               |\n");
    printf("----------------------------------------------------------------------------\n");
    for (int i = 0; i < sizeof(schedulelist)/sizeof(schedulelist[0]) && schedulelist[i].name[0] != '\0'; i++) {
        if (strcmp(id,schedulelist[i].doctorid) == 0 && schedulelist[i].availability[0] == 'P') {
            for (int j = 0; j < sizeof(ehrlist)/sizeof(ehrlist[0]) && ehrlist[j].patientid[0] != '\0'; j++) {
                if (strcmp(schedulelist[i].availability,ehrlist[j].patientid) == 0) {   
                    printf("| Patient ID          : %-50s |\n",ehrlist[j].patientid);
                    printf("| Patient Name        : %-50s |\n",ehrlist[j].name);
                    printf("| Gender              : %-50s |\n",ehrlist[j].gender);
                    printf("| Allergy             : %-50s |\n",ehrlist[i].allergy);
                    printf("| Medical History     : %-50s |\n",ehrlist[j].medicalhistory);
                    printf("| Medication          : %-50s |\n",ehrlist[j].medication);

                    if (strlen(ehrlist[j].diagnosticreport) > 5) {
                        char *token = strtok(ehrlist[j].diagnosticreport," ");
                        printf("| Body Temperature °C : %-50s |\n",token);

                        token = strtok(NULL," ");
                        printf("| Blood Pressure      : %-50s |\n",token);

                        token = strtok(NULL,",");
                        printf("| Pulse Rate          : %-50s |\n",token);
                    }
                    else {
                        printf("| Body Temperature °C : %-50s |\n","");
                        printf("| Blood Pressure      : %-50s |\n","");
                        printf("| Pulse Rate          : %-50s |\n","");
                    }
                }
            }
        }
    }

    // Get medication name from inventory.txt and compare with medication name from ehr.txt
    printf("----------------------------------------------------------------------------\n");
    printf("|                          Treatment Trends                                |\n");
    printf("----------------------------------------------------------------------------\n");
    for (int i = 0; i < sizeof(inventorylist)/sizeof(inventorylist[0]) && inventorylist[i].medicineid[0] != '\0'; i++) {
        char valid = 'Y';
        if (inventorylist[i].medicineid[0] == 'M') {
            for (int j = 0; j < sizeof(ehrlist)/sizeof(ehrlist[0]) && ehrlist[j].patientid[0] != '\0'; j++) {
                if (strcmp(inventorylist[i].name,ehrlist[j].medication) == 0) {
                    count++;
                }
            }

            // Dupe Check
            for (int k = 0; k < sizeof(inventorylist)/sizeof(inventorylist[0]) && inventorylist[k].medicineid[0] != '\0'; k++) {
                if (strcmp(inventorylist[k].name,inventorylist[i].name) == 0 && i > k) { 
                    valid = 'N';
                    break;
                }
            }

            if (valid == 'Y') {
                printf("| %-20s prescribed  : %-37d |\n",inventorylist[i].name,count);
            }
            count = 0;  
        }
    }
    printf("----------------------------------------------------------------------------\n\n");

    fclose(ehrtxt);fclose(inventorytxt);fclose(scheduletxt);
}

int DoctorMenu(char *id) {
    int input;

    do {
        printf("-------------------------------\n");
        printf("| What would you like to do ? |\n");
        printf("-------------------------------\n");
        printf("| 0. Logout                   |\n");
        printf("| 1. Manage Doctor Schedule   |\n");
        printf("| 2. Manage EHR               |\n");
        printf("| 3. Reporting & Diagnostics  |\n");
        printf("-------------------------------\n");

        printf("Enter the number corresponding to the action: ");
        scanf("%d",&input); 

        switch (input)  {
            case 0:
                printf("\nLogging out, redirecting to login menu.\n");
                return 0;
            case 1:
                DoctorScheduleManager(id);
                break;
            case 2:
                ElectronicHealthRecords();
                break;
            case 3:
                Report(id);
                break;
            default:
                printf("\nPlease enter a valid input!\n");
                while ((getchar()) != '\n');
                break;
        }
    } while (input != 0);
    return 0;
}

#endif