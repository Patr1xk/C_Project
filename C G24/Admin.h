#ifndef Admin_H
#define Admin_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

#include "Doctor.h" //include doctor schedule struct

typedef struct  {
    char userid[50];
    char fullname[50];
    char gender;
    char dob[50];
    char contactnum[50];
    char username[50];
    char password[50];
    char history[256];
    char insurance;
    char roomunit[50];
}User;

typedef struct {
    char ID[256];
    char name[256];
} List;

typedef struct{
    char medicinename[256];
    char medicineid[256];
    char supplyname[256];
    char supplyid[256];
}Inventory;

void peakappointments(schedulestruct timeslot[]) {
    char line[256];
    int size = 0;

    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    // Read file and store data in the timeslot array
    while (fgets(line, sizeof(line), file) != NULL && size < 256) {
        char *token = strtok(line, ",");
        
        if (token != NULL) strcpy(timeslot[size].doctorid, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].name, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].day, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].timeslot, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].availability, token);

        size++;
    }
    fclose(file);

    int i = 0;
    int count[256] = {0}; // Initialize all elements to 0
    char exist_timeslot[256][256]; // Array to store unique time slots
    int unique_timeslot_count = 0;
    int maxnum = 0;
    char peakTimeSlots[256][256];
    int peakCount = 0;

    printf("\n<<Peak Appointment>>\n");

    // Iterate through the timeslot array to count appointments for each time slot
    for (int j = 0; j < size; j++) {
        int found = 0;
        // Check if this time slot already exists in the exist_timeslot array
        for (int k = 0; k < unique_timeslot_count; k++) {
            if (strcmp(timeslot[j].timeslot, exist_timeslot[k]) == 0) {
                count[k]++;
                found = 1;
                break;
            }
        }
        // If the time slot is not found, add it to the exist_timeslot array
        if (!found) {
            strcpy(exist_timeslot[unique_timeslot_count], timeslot[j].timeslot);
            count[unique_timeslot_count]++;
            unique_timeslot_count++;
        }
    }

    // Find the maximum number of appointments
    for (int k = 0; k < unique_timeslot_count; k++) {
        if (count[k] > maxnum) {
            maxnum = count[k];
        }
    }

    // Collect all time slots with the maximum number of appointments
    for (int k = 0; k < unique_timeslot_count; k++) {
        if (count[k] == maxnum) {
            strcpy(peakTimeSlots[peakCount], exist_timeslot[k]);
            peakCount++;
        }
    }

    // Print all peak time slots
    printf("The peak time slot(s) are:\n");
    for (int k = 0; k < peakCount; k++) {
        printf("%s with %d appointment(s)\n", peakTimeSlots[k], maxnum);
    }
}

void staffhours(schedulestruct timeslot[256], int size) {
    List staffing[256];
    int startingtime;
    char inputdoctorid[256];
    char inputnurseid[256];
    char doctorname[256];
    char nursename[256];
    char day[256];
    char time[256];
    char line[256];
    int found_doctor = 0;
    int found_nurse = 0;
    FILE *userfile;
    FILE *schedule;
    int DocAvailable = 1;
    int NurseAvailable = 1;
    int i = 0;
    int staffSize = sizeof(staffing)/sizeof(staffing[0]);

    userfile = fopen("userdata.txt", "r");
    if (userfile == NULL) {
        printf("Error opening file. \n");
        return;
    }
    printf("\n<<Adjust Staffing Hours>>");

    //read doctors
    
    while (fgets(line, sizeof(line), userfile)) 
    {
        line[strcspn(line, "\n")] = '\0';
        char *token = strtok(line, ",");

        if (token != NULL) strcpy(staffing[i].ID, token);
        token = strtok(NULL, ",");

        if (token != NULL) strcpy(staffing[i].name, token);
        
        i++;
    }

    
    while(1)
    {
        printf("\nDoctor List:\n");
        for(int i = 0; i < staffSize && staffing[i].ID[0] != '\0' ; i++)
        {
            if(staffing[i].ID[0] == 'D')
            {
                printf("%s,%s\n", staffing[i].ID, staffing[i].name);
            }
        }

        //enter doctor ID
        printf("\nEnter ID of the doctor you would like to assign: ");
        scanf("%s", inputdoctorid);
        inputdoctorid[0] = toupper (inputdoctorid[0]);

        for (int i = 0; i < staffSize; i++) 
        {
            if (strcmp(staffing[i].ID, inputdoctorid) == 0) 
            {
                strcpy(doctorname, staffing[i].name);
                printf("Doctor: %s\n", doctorname);
                found_doctor = 1;
                break;
            }
        }

        if(found_doctor)
        {
            break;
        }

        //retry again
        if(!found_doctor)
        {
            printf("Doctor ID not found. Please reschedule!\n");
        }
    }

    while(1)
    {
        printf("\nNurse List:\n");
        for(int i = 0; i < staffSize && staffing[i].ID[0] != '\0' ; i++)
        {
            if(staffing[i].ID[0] == 'N')
            {
                printf("%s,%s\n", staffing[i].ID, staffing[i].name);
            }
        }

        printf("\nEnter ID of the nurse you would like to assign: ");
        scanf("%s", inputnurseid);
        inputnurseid [0] = toupper (inputnurseid[0]);

        for (int i = 0; i < staffSize; i++) 
        {
            if (strcmp(staffing[i].ID, inputnurseid) == 0) 
            {
                strcpy(nursename, staffing[i].name);
                printf("Nurse: %s\n", nursename);
                found_nurse = 1;
                break;
            }
        }

        if(found_nurse)
        {
            break;
        }

        if(!found_nurse)
        {
            printf("Nurse ID not found. Please reschedule!\n");
        }
    }
    
    //enter starting time
    while (1) 
    {
        printf("\nEnter the starting time of the appointment slot: ");
        scanf("%d", &startingtime);

        if (startingtime >= 1 && startingtime <= 4) {
            sprintf(time, "%dpm-%dpm", startingtime, startingtime + 1);
            printf("Duration of the appointment slot: %s\n", time);
            break;
        } else if (startingtime >= 8 && startingtime <= 10) {
            sprintf(time, "%dam-%dam", startingtime, startingtime + 1);
            printf("Duration of the appointment slot: %s\n", time);
            break;
        } else if (startingtime == 11) {
            sprintf(time, "11am-12pm");
            printf("Duration of the appointment slot: %s\n", time);
            break;
        } else if (startingtime == 12) {
            sprintf(time, "12pm-1pm");
            printf("Duration of the appointment slot: %s\n", time);
            break;
        } else {
            printf("Please enter time during working hours (8am - 5pm).\n");
        }
    }
    
    //enter day
    while (1) 
    {
        printf("\nChoose one day between Monday and  Sunday:");
        printf("\nEnter the appointment day: ");
        scanf("%s", day);
        day[0] = toupper (day[0]);

        if (strcmp(day, "Monday") == 0 || strcmp(day, "Tuesday") == 0 ||
            strcmp(day, "Wednesday") == 0 || strcmp(day, "Thursday") == 0 ||
            strcmp(day, "Friday") == 0 || strcmp(day, "Saturday") == 0 ||
            strcmp(day, "Sunday") == 0) 
        {
            printf("Appointment day: %s\n", day);
            break;
        } 
        else 
        {
            printf("Please enter a valid day from Monday to Sunday.\n");
        }
    }

    for(int i = 0; i < size ; i++)
    {
        if( strcmp(day, timeslot[i].day) == 0 && 
        strcmp(time, timeslot[i].timeslot) == 0 && 
        strcmp(inputdoctorid, timeslot[i].doctorid) == 0)
        {
            printf("The doctor not available for this time!\n");
            DocAvailable = 0;
            break;
        }
    }

    for(int i = 0; i < size; i++)
    {
        if( strcmp(day, timeslot[i].day) == 0 && 
        strcmp(time, timeslot[i].timeslot) == 0 && 
        strcmp(inputnurseid, timeslot[i].nurseid) == 0)
        {
            printf("The nurse not available for this time!\n");
            NurseAvailable = 0;
            break;
        }
    }

    if(DocAvailable && NurseAvailable)
    {
        printf("\nAppointment Scheduled:");
        printf("\n%s,%s,%s,%s,Y,%s\n", inputdoctorid, doctorname, day, time, inputnurseid);

        schedule = fopen("schedule.txt", "a");
        if (schedule == NULL) {
            printf("Error opening file.\n");
            return;
        }
        fprintf(schedule, "%s,%s,%s,%s,Y,%s,\n", inputdoctorid, doctorname, day, time, inputnurseid);
        fclose(schedule);
    }
}

void adminappointment(schedulestruct timeslot[256], int size)
{
    int appchoice;
    FILE *file;

    while (1)
    {
        printf("\n<<Appointments Trends>>");
        printf("\n1. Identify Peak Appointments\n2. Adjust Staffing Schedule\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &appchoice);
        getchar();

        switch (appchoice)
        {
        case 1:
            file = fopen("schedule.txt", "r");
            if (file == NULL)
            {
                printf("Error opening file.\n");
                break;
            }

            peakappointments(timeslot);
            printf("\n");
            fclose(file);
            break;

        case 2:
            file = fopen("schedule.txt", "r");
            if (file == NULL)
            {
                printf("Error opening file.\n");
                break;
            }

            staffhours(timeslot, size);
            printf("\n");
            fclose(file);
            break;

        case 3:
            printf("Exiting program.\n");
            return;

        default:
            printf("Please enter a valid choice.\n");
        }
    }
}

void admininventory()
{
    Inventory inventory[256];
    int invchoice;
    char searchitem[256];
    char invline[256];
    int mcount = 0;
    int scount = 0;
    FILE *inventoryfile;

    while (1)
    {
        printf("\n<<Supplies & Medications>>");
        printf("\n1. View All Inventory\n2. Keyword Search in Inventory\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &invchoice);
        getchar();

        switch (invchoice)
        {
        case 1:
            inventoryfile = fopen("inventory.txt", "r");
            if (inventoryfile == NULL)
            {
                printf("Error opening file.\n");
                break;
            }

            while (fgets(invline, sizeof(invline), inventoryfile)) 
            {
                invline[strcspn(invline, "\n")] = '\0';
                char *token = strtok(invline, ",");

                if (token[0] == 'M') 
                {
                    strcpy(inventory[mcount].medicineid, token);
                    token = strtok(NULL, ",");

                    strcpy(inventory[mcount].medicinename, token);
                    mcount++;
                }
            }
            
            printf("\n<<Medicine List>>\n");
            for(int i = 0; i < sizeof(inventory) && inventory[i].medicineid[0] != '\0' ; i++)
            {
                printf("%s,%s\n", inventory[i].medicineid, inventory[i].medicinename);
            }

            rewind(inventoryfile);

            while (fgets(invline, sizeof(invline), inventoryfile)) 
            {
                invline[strcspn(invline, "\n")] = '\0';
                char *token = strtok(invline, ",");

                if (token[0] == 'S') 
                {
                    strcpy(inventory[scount].supplyid, token);
                    token = strtok(NULL, ",");

                    strcpy(inventory[scount].supplyname, token);
                    scount++;
                }
            }
            
            printf("\n<<Supplies List>>\n");
            for(int i = 0; i < sizeof(inventory) && inventory[i].supplyid[0] != '\0' ; i++)
            {
                printf("%s,%s\n", inventory[i].supplyid, inventory[i].supplyname);
            }

            printf("\n");
            fclose(inventoryfile);
            break;

        case 2:
            inventoryfile = fopen("inventory.txt", "r");
            if (inventoryfile == NULL)
            {
                printf("Error opening file.\n");
                break;
            }

            while (1)
            {
                int invalid = 0;

                printf("Enter Inventory ID you would like to search: ");
                scanf("%255s", searchitem);  //limit input length to avoid buffer overflow
                searchitem[0] = toupper (searchitem[0]);

                if (strlen(searchitem) != 4)
                {
                    printf("Please enter a valid Inventory ID!\n");
                    invalid = 1;
                }

                if(!invalid)
                {
                    int found = 0;
                    printf("\nYour search result: \n");
                    while (fgets(invline, sizeof(invline), inventoryfile) != NULL)
                    {
                        
                        if (strstr(invline, searchitem) != NULL) //find searchitem in lines
                        {
                            printf("%s", invline);
                            found = 1;
                        }
                    }

                    if (!found)
                    {
                        printf("Item not found in inventory.\n");
                    }
                    
                    fclose(inventoryfile);
                    break;
                }
            }
            break;
        case 3:
            printf("Exiting program.\n");
            return;

        default:
            printf("Please enter a valid choice.\n");
        }
    }
}

void CopyIntoArray(User user[256], char filename[]) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    char info[1024]; // Buffer to read lines from the file
    int i = 0; // Index for user array

    // Read each line from the file
    while (fgets(info, sizeof(info), fptr) != NULL && i < 256) {
        // Tokenize the line
        char *token = strtok(info, ",");
        if (token != NULL) 
        {
            strcpy(user[i].userid, token);
            token = strtok(NULL, ",");
        }
            if (token != NULL) 
            {
                strcpy(user[i].fullname, token);
                token = strtok(NULL, ",");
            }
                if (token != NULL) 
                {
                    user[i].gender = token[0]; // Assuming gender is a single character
                    token = strtok(NULL, ",");
                }
                    if (token != NULL) 
                    {
                        strcpy(user[i].dob, token);
                        token = strtok(NULL, ",");
                    }
                        if (token != NULL) 
                        {
                            strcpy(user[i].contactnum, token);
                            token = strtok(NULL, ",");
                        }
                            if (token != NULL) 
                            {
                                strcpy(user[i].username, token);
                                token = strtok(NULL, ",");
                            }
                                if (token != NULL) 
                                {
                                    strcpy(user[i].password, token);
                                    token = strtok(NULL, ",");
                                }
                                    if (token != NULL) 
                                    {
                                        strcpy(user[i].roomunit, token);
                                        token = strtok(NULL, ",");
                                    }
                                        if (token != NULL) 
                                        {
                                            strcpy(user[i].history, token);
                                            token = strtok(NULL, ",");
                                        }
                                            if (token != NULL) 
                                            {
                                                user[i].insurance = token[0];
                                                i++;  // Move to the next user
                                            }
        }
        fclose(fptr); 
    }

void dateCheck(User*user) 
{
    char dob[11];
    char yearstr[5];
    int year;
    char monthstr[3];
    int month;
    char datestr[3];
    int date;

    bool validDate = false;

    while (!validDate) //true
    {
        printf("Date of birth (YYYY-MM-DD): ");
        scanf("%s", dob);

        if (strlen(dob) != 10 || dob[4] != '-' || dob[7] != '-') 
        {
            printf("Invalid birth date format! Please enter in YYYY-MM-DD format.\n");
            continue;
        }

        strncpy(yearstr, dob, 4);
        yearstr[4] = '\0';
        year = atoi(yearstr); //str to int

        strncpy(monthstr, dob + 5, 2);
        monthstr[2] = '\0';
        month = atoi(monthstr);

        strncpy(datestr, dob + 8, 2);
        datestr[2] = '\0';
        date = atoi(datestr);

        int monthday[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) 
        {
            monthday[1] = 29;
        }

        if (year > 2024) 
        {
            printf("Invalid year input!\n");
        } else if (year >= 2012) 
        {
            printf("Children born in or after 2012 must be registered under parent's account.\n");
        } else if (year < 1939) 
        {
            printf("Users born before 1939 must be registered under supervisor/guardian's account.\n");
        } else if (month < 1 || month > 12) 
        {
            printf("There are only 12 months.\n");
        } else if (date < 1 || date > monthday[month - 1]) 
        {
            printf("Invalid day for the given month.\n");
        } else 
        {
            validDate = true;
            strcpy(user->dob, dob);
        }
    }
}

void userInfo(User *user, int size) 
{
    getchar(); //clear the input buffer//consume the newline character left by previous input
    printf("Enter name: ");
    fgets(user->fullname, sizeof(user->fullname), stdin);
    user->fullname[strcspn(user->fullname, "\n")] = '\0';

    printf("Gender (F/M): ");
    scanf(" %c", &user->gender);
    user->gender = toupper(user->gender);
    while (user->gender != 'F' && user->gender != 'M') 
    {
        printf("Invalid input! Please enter F/M only.\n");
        printf("Gender (F/M): ");
        while (getchar() != '\n'); //clear input buffer
        scanf(" %c", &user->gender);
        user->gender = toupper(user->gender);
    }

    dateCheck(user);

    int tryContact;
    char testNum[256];
    int c;
    
    while(1)
    {
        printf("Contact Number (+60): ");
        
        if(scanf("%d", &tryContact) == 1) //integer
        {
            sprintf(testNum, "%d", tryContact); //become string
        }
        else //if not integer
        {
            printf("Numbers only!\n");
            while((c = getchar()) != '\n' && c != EOF);//clear previous input
            continue;
        }

        if(strlen(testNum) != 9 && strlen(testNum) != 10)
        {
            printf("Please enter valid contact number.\n");
            continue;
        }
        else
        {
            strcpy(user->contactnum, testNum);
            break;
        }
    }
    
    char Newusername[50];
    while(1) //true
    {
        bool usernameduplicate = false;

        printf("Set username: ");
        scanf("%s", Newusername);

        for(int i = 0; i < 256; i++)  //scan all array to see whther got duplication
        {
            if (strcmp(user[i].username, Newusername) == 0) //equal
            {     
                usernameduplicate = true;  //true
                printf("Duplicated username!\n");
                break;
            }
        }
    
        if(!usernameduplicate)
        {
            for(int i = 0; i < 256; i++)  
            {
                if(strcmp(user[i].username,Newusername) != 0)  //not equal
                {
                    strcpy(user[i].username, Newusername); 
                    printf("Username set!\n");
                    break; //break for loop
                }
            }
            break; //break while loop
        }   
    }

    printf("Set password: ");
    scanf("%s", user->password);
}

int adminregister() {
    char historyInput;
    char position;
    int value;
    int allergy;
    char hospitalized;
    User user[256];
    char filename[] = "userdata.txt";
    int size = sizeof(user)/sizeof(user[0]);
    CopyIntoArray(user, filename);

    printf("<<Registration page>>\n");
    printf("Select the position:\n");
    printf("1. Patient (P)\n2. Doctor (D)\n3. Staff Nurse (N)\n4. Admin (A)\n");
    printf("Position (P/D/N/A): ");
    scanf(" %c", &position);
    position = toupper(position);
    while (position != 'P' && position != 'D' && position != 'N' && position != 'A') {
        printf("Invalid option! Please enter only P/D/N/A.\n");
        printf("Position (P/D/N/A): ");
        scanf(" %c", &position);
        position = toupper(position);
    }

    if (position == 'D' || position == 'N' || position == 'A') {
        userInfo(user,size);
    } else {
        userInfo(user,size);

        printf("Have any allergies? (Y/N): ");
        scanf(" %c", &historyInput);
        historyInput = toupper(historyInput);
        while (historyInput != 'Y' && historyInput != 'N') {
            printf("Invalid input! Please enter Y/N only.\n");
            printf("Have any allergies? (Y/N):  ");
            while (getchar() != '\n'); //clear input buffer
            scanf(" %c", &historyInput);
            historyInput = toupper(historyInput);
        }

        if (historyInput == 'Y') {
        do {
            printf("What allergies?\n1. Medicine Allergy\n2. Food Allergy\n3. Pollen Allergy\n");
            printf("Enter type of allergies (1-3): ");
            scanf("%d", &allergy);
            if (allergy == 1) {
                strcpy(user->history, "Medicine Allergy");
            } else if (allergy == 2) {
                strcpy(user->history, "Food Allergy");
            } else if (allergy == 3) {
                strcpy(user->history, "Pollen Allergy");
            } else {
                printf("Invalid input. Please enter 1-3 only.\n");
            }
        } while (allergy != 1 && allergy != 2 && allergy != 3);}

        if (historyInput == 'N') {
            printf("No allergies reported.\n");
            strcpy(user->history, "N");
        } else {
            printf("Allergy type: %s\n", user->history);
        }


        printf("Have insurance claim? (Y/N): ");
        scanf(" %c", &user->insurance);
        user->insurance = toupper(user->insurance);
        while (user->insurance != 'Y' && user->insurance != 'N') {
            printf("Invalid input! Please enter Y/N only.\n");
            printf("Have insurance claim? (Y/N):  ");
            while (getchar() != '\n'); //clear input buffer
            scanf(" %c", &user->insurance);
            user->insurance = toupper(user->insurance);
        }

        printf("Need to be hospitalized? (Y/N): ");
        scanf(" %c", &hospitalized);
        hospitalized = toupper(hospitalized);
        while (hospitalized != 'Y' && hospitalized != 'N') {
            printf("Invalid input! Please enter Y/N only.\n");
            printf("Need to be hospitalized? (Y/N): ");
            while (getchar() != '\n'); //clear input buffer
            scanf(" %c", &hospitalized);
            hospitalized = toupper(hospitalized);
        }
    }

    int Rvalue;
    if (hospitalized == 'Y') 
    {
        int roomMaxNum = 50;
        srand(time(NULL));
        Rvalue = rand() % roomMaxNum + 1;
        printf("Room Unit is: R %03d\n", Rvalue);
    }

    int maxnum = 1000;
    srand(time(NULL));
    value = rand() % maxnum + 1;
    printf("Your ID is: %c %03d\n", position, value);

    FILE *fptr;
    fptr = fopen("userdata.txt", "a");

    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    } else {
        if (hospitalized == 'Y') {
            fprintf(fptr, "%c%03d,%s,%c,%s,+60%s,%s,%s,%s,%c,R%03d,\n", position, value, user->fullname, user->gender, user->dob, user->contactnum, user->username, user->password, user->history, user->insurance, Rvalue);
        } 
        else if (hospitalized == 'N') {
            fprintf(fptr, "%c%03d,%s,%c,%s,+60%s,%s,%s,%s,%c,\n", position, value, user->fullname, user->gender, user->dob, user->contactnum, user->username, user->password, user->history, user->insurance);
        } 
        else {
            fprintf(fptr, "%c%03d,%s,%c,%s,+60%s,%s,%s,\n", position, value, user->fullname, user->gender, user->dob, user->contactnum, user->username, user->password);
        }
        fclose(fptr);
    }
    printf("Registration successful!\n");
    return 0;
    }

void admin()
{
    schedulestruct timeslot[256];
    int size = 0;
    char line[256];
    int useroption;
    int c;

    FILE *file = fopen("schedule.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    // Read file and store data in the timeslot array
    while (fgets(line, sizeof(line), file) != NULL && size < 256) {
        char *token = strtok(line, ",");
        
        if (token != NULL) strcpy(timeslot[size].doctorid, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].name, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].day, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].timeslot, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].availability, token);
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(timeslot[size].nurseid, token);

        size++;
    }
    fclose(file);

    do
    {
        printf("\n<<Hospital Administrator Menu>>\n");
        printf("1. Register New User \n");
        printf("2. Appointment Trends \n");
        printf("3. Inventory Management \n");
        printf("4. Exit\n");
        printf("Enter your option:");
        scanf("%d", &useroption);
        while ((c = getchar()) != '\n' && c != EOF);

        switch (useroption)
        {
            case 1:
                adminregister();
                break;

            case 2:
                adminappointment(timeslot, size);
                break;

            case 3:
                admininventory();
                break;

            case 4:
                printf("Exiting program.\n");
                return;

            default:
                printf("Please enter a valid choice.\n");
        }
    }while (useroption!=4);
}

#endif