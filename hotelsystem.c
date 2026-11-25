
#include <stdio.h>
#include <string.h>
#include <ctype.h>
struct Customer {
    int id;
    char name[50];
    char phone[15];
};
int readNumber(int *num) {
    char input[100];
    char extra;
    if(fgets(input,sizeof(input),stdin)==NULL)
        return 0;
    if(sscanf(input,"%d %c",num,&extra)!=1)
        return 0;
    return 1;
}
void getPhone(char phone[]) {
    char input[100];
    int valid,i;
    while(1) {
        printf("Enter 10-digit phone number: ");
        if(fgets(input,sizeof(input),stdin)==NULL)
            continue;
        input[strcspn(input,"\n")]='\0';
        if(strlen(input)==10) {
            valid=1;
            for(i=0;i<10;i++) {
                if(!isdigit((unsigned char)input[i])) {
                    valid=0;
                    break;
                }
            }
            if(valid) {
                strcpy(phone,input);
                return;
            }
        }
        printf("Invalid phone number! Enter exactly 10 digits.\n");
    }
}
int main() {
    struct Customer c[50];
    int room[10]={0};
    int days[10]={0};
    int customerId[10]={0};
    int choice,i,id=0;
    int roomNo,index;
    float bill;
    FILE *fp;
    fp=fopen("customers.txt","r");
    if(fp!=NULL) {
        while(id<50 && fscanf(fp,"%d|%49[^|]|%14s",&c[id].id,c[id].name,c[id].phone)==3) {
            id++;
        }
        fclose(fp);
    }
    fp=fopen("rooms.txt","r");
    if(fp!=NULL) {
        for(i=0;i<10;i++) {
            if(fscanf(fp,"%d %d %d",&room[i],&days[i],&customerId[i])!=3)
                break;
        }
        fclose(fp);
    }
    do {
        printf("\n===== HOTEL RESERVATION SYSTEM =====\n");
        printf("1. Register Customer\n");
        printf("2. Book Room\n");
        printf("3. Check Room Availability\n");
        printf("4. Cancel Booking\n");
        printf("5. Checkout and Bill\n");
        printf("6. Occupancy Status\n");
        printf("7. Inventory Report\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        if(!readNumber(&choice) || choice<1 || choice>8) {
            printf("Invalid input! Enter a number from 1 to 8.\n");
            choice=0;
            continue;
        }
        switch(choice) {
            case 1:
                if(id>=50) {
                    printf("Customer limit reached!\n");
                    break;
                }
                c[id].id=id+1;
                printf("Enter customer name: ");
                if(fgets(c[id].name,sizeof(c[id].name),stdin)==NULL)
                    break;
                c[id].name[strcspn(c[id].name,"\n")]='\0';
                if(strlen(c[id].name)==0 || strchr(c[id].name,'|')!=NULL) {
                    printf("Invalid customer name!\n");
                    break;
                }
                getPhone(c[id].phone);
                fp=fopen("customer.txt","a");
                if(fp==NULL) {
                    printf("File error!\n");
                    break;
                }
                fprintf(fp,"%d|%s|%s\n",c[id].id,c[id].name,c[id].phone);
                fclose(fp);
                id++;
                printf("Customer registered successfully!\n");
                printf("Customer ID: %d\n",id);
                break;
            case 2:
                printf("Enter customer ID: ");
                if(!readNumber(&index)) {
                    printf("Invalid input!\n");
                    break;
                }
                if(index<1 || index>id) {
                    printf("Invalid customer ID!\n");
                    break;
                }
                printf("Enter room number (101-110): ");
                if(!readNumber(&roomNo) || roomNo<101 || roomNo>110) {
                    printf("Invalid room number!\n");
                    break;
                }
                i=roomNo-101;
                if(room[i]==1) {
                    printf("Room already booked!\n");
                    break;
                }
                printf("Enter number of days: ");
                if(!readNumber(&days[i]) || days[i]<=0) {
                    printf("Invalid number of days!\n");
                    days[i]=0;
                    break;
                }
                room[i]=1;
                customerId[i]=index;
                fp=fopen("rooms.txt","w");
                if(fp==NULL) {
                    printf("File error!\n");
                    room[i]=0;
                    days[i]=0;
                    customerId[i]=0;
                    break;
                }
                for(int j=0;j<10;j++)
                    fprintf(fp,"%d %d %d\n",room[j],days[j],customerId[j]);
                fclose(fp);
                printf("Room booked successfully!\n");
                break;
            case 3:
                printf("\nRoom Availability:\n");
                for(i=0;i<10;i++) {
                    printf("Room %d: ",101+i);
                    if(room[i]==0)
                        printf("Available\n");
                    else
                        printf("Booked\n");
                }
                break;
            case 4:
                printf("Enter room number to cancel: ");
                if(!readNumber(&roomNo) || roomNo<101 || roomNo>110) {
                    printf("Invalid room number!\n");
                    break;
                }
                i=roomNo-101;
                if(room[i]==1) {
                    room[i]=0;
                    days[i]=0;
                    customerId[i]=0;
                    fp=fopen("rooms.txt","w");
                    if(fp==NULL) {
                        printf("File error!\n");
                        break;
                    }
                    for(int j=0;j<10;j++)
                        fprintf(fp,"%d %d %d\n",room[j],days[j],customerId[j]);
                    fclose(fp);
                    printf("Booking cancelled successfully!\n");
                } else {
                    printf("Room is not booked!\n");
                }
                break;
            case 5:
                printf("Enter room number for checkout: ");
                if(!readNumber(&roomNo) || roomNo<101 || roomNo>110) {
                    printf("Invalid room number!\n");
                    break;
                }
                i=roomNo-101;
                if(room[i]==1) {
                    index=customerId[i]-1;
                    if(index<0 || index>=id) {
                        printf("Customer record not found!\n");
                        break;
                    }
                    bill=days[i]*1500;
                    printf("\nCustomer Name: %s\n",c[index].name);
                    printf("Phone Number: %s\n",c[index].phone);
                    printf("Room Number: %d\n",roomNo);
                    printf("Days Stayed: %d\n",days[i]);
                    printf("Total Bill: Rs. %.2f\n",bill);
                    fp=fopen("bill_history.txt","a");
                    if(fp==NULL) {
                        printf("Unable to save bill history!\n");
                        break;
                    }
                    fprintf(fp,"Customer ID: %d\n",c[index].id);
                    fprintf(fp,"Customer Name: %s\n",c[index].name);
                    fprintf(fp,"Phone Number: %s\n",c[index].phone);
                    fprintf(fp,"Room Number: %d\n",roomNo);
                    fprintf(fp,"Days Stayed: %d\n",days[i]);
                    fprintf(fp,"Total Bill: Rs. %.2f\n",bill);
                    fprintf(fp,"-----------------------------\n");
                    fclose(fp);
                    room[i]=0;
                    days[i]=0;
                    customerId[i]=0;
                    fp=fopen("rooms.txt","w");
                    if(fp==NULL) {
                        printf("File error while updating rooms!\n");
                        break;
                    }
                    for(int j=0;j<10;j++)
                        fprintf(fp,"%d %d %d\n",room[j],days[j],customerId[j]);
                    fclose(fp);
                    printf("Bill saved to bill_history.txt\n");
                    printf("Checkout completed!\n");
                } else {
                    printf("Room is not booked!\n");
                }
                break;
            case 6:
                printf("\n===== OCCUPANCY STATUS =====\n");
                for(i=0;i<10;i++) {
                    printf("Room %d: ",101+i);
                    if(room[i]==1)
                        printf("Occupied\n");
                    else
                        printf("Vacant\n");
                }
                break;
            case 7:
                printf("\n===== INVENTORY REPORT =====\n");
                for(i=0;i<10;i++) {
                    printf("Room %d - ",101+i);
                    if(room[i]==1)
                        printf("Booked\n");
                    else
                        printf("Available\n");
                }
                break;
            case 8:
                printf("Thank you for using our system!\n");
                break;
        }
    } while(choice!=8);
    return 0;
}
