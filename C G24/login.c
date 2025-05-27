#include <stdio.h>
#include <string.h>
#include <stdbool.h>
//test`
#include "Nurse.h"
#include "Admin.h"
#include "Doctor.h"
#include "Patient.h"


//define login structure
typedef struct 
{   
    char userID[100];
    char username[100];
    char password[100];
}UserInfo;

//create file
void createAllFile()
{
    char buffer[500];
    //USER DATA FILE
    //read file first
    FILE *userdata = fopen("userdata.txt", "r");
    if(userdata == NULL) //if file doesn't exist then create
    {
        printf("Creating user data file...\n");

        userdata = fopen("userdata.txt", "w");//write login info
        fprintf(userdata, 
                "A266,Jane Tan,F,2004-08-09,+60119110111,jane,tan,\n"
                "D080,Yen Hau,M,2002-01-10,+601102929101,yen,hau123,\n"
                "D178,Peter Loo,M,1999-03-20,+601913211829,pet,loohiii,\n"
                "D985,Sam Ong,M,2001-11-12,+601456181829,ong,sam123,\n"
                "D909,Lim Pong,M,2001-11-11,+601456181830,lim,pp,\n"
                "N155,Patrick Woo,M,2003-08-30,+601918181829,pat,woo,\n"
                "N230,Weilson Lon,M,2002-06-23,+601918181123,wilson,lala,\n"
                "P220,Alex Tan,M,2001-11-11,+60102891091,alex,tan,Pollen Allergy,Y,\n"
                "P409,William Lee,M,1999-11-10,+601029010101,willl,um,N,Y,R009,\n"
                "P878,Ng Hwee Meng,F,2004-02-07,+60119282933,meng123,ng,Food Allergy,N,R028,\n"
                );
        
        fclose(userdata);
    }

    //INVENTORY FILE
    FILE *inventory = fopen("inventory.txt", "r");
    if(inventory == NULL)
    {
        printf("Creating inventory file...\n");
        inventory = fopen("inventory.txt", "w");

        fprintf(inventory, 
                        "M001,Ibuprofen,Fever,50,25,08/2024,-,\n"
                        "M002,Acetaminophen,Fever,30,15,09/2024,-,\n"
                        "M003,Dextromethorphan,Cough,25,15,09/2024,-,\n"
                        "M004,Omeprazole,Acid Reflux,50,25,09/2024,-,\n"
                        "M005,Aspirin,Pain Relief,100,10,12/2024,-,\n"
                        "M006,Loratadine,Allergies,60,20,03/2025,-,\n"
                        "M007,Amoxicillin,Infection,80,50,11/2024,-,\n"
                        "M008,Metformin,Diabetes,90,30,05/2025,-,\n"
                        "M009,Simvastatin,Cholesterol,120,40,09/2025,-,\n"
                        "M010,Albuterol,Asthma,70,35,07/2024,-,\n"
                        "S001,Surgical Mask, ,15,50,08/2024,-,\n"
                        "S002,Alcohol Swabs, ,25,35,12/2025,-,\n"
                        "S003,Gloves, ,1,40,09/2026,Low,\n"
                        "S004,Film Dressing, ,20,30,06/2026,-,\n"
                        "S005,Gauze Pads, ,200,15,11/2025,-,\n"
                        "S006,Bandages, ,150,25,10/2024,-,\n"
                        "S007,Face Shield, ,10,60,07/2024,-,\n"
                        "S008,Thermometer, ,5,45,05/2025,Low,\n"
                        "S009,Hand Sanitizer, ,50,30,08/2025,-,\n"
                        "S010,Cotton Balls, ,100,20,06/2025,-,\n"
                );
        
        fclose(inventory);
    }

    //SCHEDULE FILE
    FILE *scheduletxt = fopen("schedule.txt","r");

    if (scheduletxt == NULL)
    {
        printf("Creating schedule file...\n");
        scheduletxt = fopen("schedule.txt","w");

        fprintf(scheduletxt,"D080,Yen Hau,Monday,9am-10am,P409,N155,\n"
                            "D080,Yen Hau,Monday,11am-12pm,Y,N155,\n"
                            "D080,Yen Hau,Monday,2pm-3pm,Y,N155,\n"
                            "D080,Yen Hau,Tuesday,3pm-4pm,Y,N155,\n"
                            "D178,Peter Loo,Wednesday,9am-10am,P878,N230,\n"
                            "D178,Peter Loo,Wednesday,10am-11am,Y,N230,\n"
                            "D178,Peter Loo,Friday,9am-10am,Y,N230,\n"
                            "D985,Sam Ong,Thursday,8am-9am,P220,N230,\n"
                            "D909,Lim Pong,Monday,4pm-5pm,Y,N155,\n"
                            );

        fclose(scheduletxt);
    }

    //EHR FILE
    FILE *ehrtxt = fopen("ehr.txt","r");

    if (ehrtxt == NULL) 
    {
        printf("Creating EHR file...\n");
        ehrtxt = fopen("ehr.txt","w");
        
        fprintf(ehrtxt, "P878,Ng Hwee Meng,F,2004-02-07,N,Food Allergy,none,Ibuprofen,none,9am-10am,N155,\n"
                        "P409,William Lee,M,1999-11-10,Y,none,none,Omeprazole,none,none,N155,\n"
                        "P220,Alex Tan,M,2001-11-11,Y,Pollen Allergy,none,Ibuprofen,none,none,N230,\n");

        fclose(ehrtxt);
    }

}

//copy info into array
void CopyInfo(UserInfo loginInfo[100], char buffer[100])
{
    int i = 0;
    FILE *userdata = fopen("userdata.txt", "r");
    while(fgets(buffer, 100, userdata) != NULL)//read file line per line
    {
        if(buffer[strlen(buffer) - 1] == '\n')
        {
            buffer[strlen(buffer) - 1] = '\0'; //change new line to null
        }
        
        char *token = strtok(buffer, ","); 
        if(token != NULL)
        {
            strcpy(loginInfo[i].userID, token);
            token = strtok(NULL, ",");
        }
        for (int i = 0; i < 4; i++)
        {
            token = strtok(NULL, ",");
        }
        if(token != NULL)
        {
            strcpy(loginInfo[i].username, token);
            token = strtok(NULL, ",");//continue
            if(token != NULL)
            {
                strcpy(loginInfo[i].password, token);
                i++;
            }
        }
    }
    fclose(userdata);
}

int main()
{
    UserInfo loginInfo[100];
    int input;
    char username[100], password[100];
    char buffer[100];
    int c;

    //create new file tht doesn't exist
    createAllFile();

    //copy txtfile into array
    CopyInfo(loginInfo,buffer);

    //menu
    do
    {
        printf("\n===Hospital Management System===\n1. Login\n2. Exit\n");
        printf("Enter option: ");
        if(scanf("%d", &input) != 1) 
        {
            printf("Please enter a valid integer!\n");
            while ((c = getchar()) != '\n' && c != EOF); // Clear the input buffer
            continue;
        }
        while ((c = getchar()) != '\n' && c != EOF);
       
        switch(input)
        {
            case 1:
                printf("<<Login Page>>\n");
                printf("Enter username: ");
                fgets(username, 100, stdin);
                username[strcspn(username, "\n")] = '\0'; 

                printf("Enter password: ");
                fgets(password, 100, stdin);
                password[strcspn(password, "\n")] = '\0'; 


                bool LoggedIn = false;
                for(int i = 0; i < sizeof(loginInfo)/sizeof(loginInfo[0]); i++)
                {   
                    if(strcmp(username, loginInfo[i].username) == 0 && strcmp(password, loginInfo[i].password) == 0) 
                    //compare to see if its equal
                    {
                        printf("Login Successful!\n");
                        LoggedIn = true; //if logged in true , break.
                       switch (loginInfo[i].userID[0]) 
                       {
                            case 'D':
                                printf("Directing to Doctor Page...\n");
                                DoctorMenu(loginInfo[i].userID);
                                break;
                            case 'A':
                                printf("Directing to Admin Page...\n");
                                admin();
                                break;
                            case 'N':
                                printf("Directing to Nurse Page...\n");
                                nurse(loginInfo[i].userID);
                                break;
                            case 'P':
                                printf("Directing to Patient Page...\n");
                                patient(loginInfo[i].userID);
                                break;
                            default:
                                printf("No role!\n");
                        }
                        break;
                    }
                }
                if(!LoggedIn)
                {
                    printf("Login Failed! Invalid username or password.\n");
                }
                break;
            case 2:
                printf("Exiting program..."); //quit program
                break;
            default:
                printf("Please enter a valid option!\n");
        }
    }while(input != 2);
}



