/*Bir kargo þirketi, müþterilerinin kargolarýný etkili bir þekilde takip etmelerini ve gönderi 
bilgilerine hýzlýca eriþmelerini saðlamak istemektedir.
 Uygulamada olacak iþlemler:
  1. Kargo Bilgilerini Depolama
  2. Takip Numarasý Oluþturma ve Hashleme
  3. Kargolarý toplu bir þekilde sisteme girme
  4. Kullanýcý tarafýndan kargo giriþi
  5. Kargo Takip Durum
  6. Kargo Teslim Etme iþlemi (Hash tablosundan silme)
  7. Kargolarý Listele (Hash tablosundaki bilgileri listeleme)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLO_BOYUTU 10

typedef struct{
    int takipNo;
    char gonderici[100];
    char alici[100];
    char kargoDurum[100];
}Kargo;

Kargo*hash_tablo[10];

int hash_f(int takipNo){
    return takipNo%TABLO_BOYUTU;
}
int double_hash_f(int takipNo,int deneme){
    return (hash_f(takipNo)+deneme*(7-(takipNo%7)))%TABLO_BOYUTU;
}

void kargoEkle(Kargo* kargo){
    int index=hash_f(kargo->takipNo);
    int deneme=1;
    while(hash_tablo[index]!=NULL){
        index=double_hash_f(kargo->takipNo,deneme);
        deneme++;
        if(deneme>TABLO_BOYUTU){
            printf("Hash tablosu dolu, kargo eklenemedi.\n");
            return;
        }
    }
    Kargo*yeniKargo=(Kargo*)malloc(sizeof(Kargo));
    if(yeniKargo == NULL){
        printf("Bellek ayrilamadi.\n");
        exit(EXIT_FAILURE);
    }
    *yeniKargo=*kargo;
    hash_tablo[index]=yeniKargo;
}

void kargolariTopluEkle(){
    FILE* dosya=fopen("veri.txt","r");
    if(dosya == NULL){
        printf("Dosya acma hatasi.\n");
        exit(EXIT_FAILURE);
    }
    Kargo kargo;
    while(fscanf(dosya, "%d %s %s %s",&kargo.takipNo,kargo.gonderici,kargo.alici,kargo.kargoDurum)!=EOF){
        kargoEkle(&kargo);
    }
    fclose(dosya);
}
void kargoGiris(){
    Kargo yeniKargo;
    printf("Takip Numarasi:");
    scanf("%d",&yeniKargo.takipNo);
    printf("Gonderici:");
    scanf("%s",yeniKargo.gonderici);
    printf("Alici:");
    scanf("%s",yeniKargo.alici);
    printf("Kargo Durumu:");
    scanf("%s",yeniKargo.kargoDurum);
    kargoEkle(&yeniKargo);
}

void kargoTakipDurumu(int takipNo){
    int index=hash_f(takipNo);
    int deneme=1;
    while(hash_tablo[index]!=NULL){
        if(hash_tablo[index]->takipNo==takipNo){
            printf("Takip No %d hash tablosunda %d adimda bulundu.\n",takipNo,deneme);
            printf("Kargo Bilgileri:\n");
            printf("Takip No:%d\nGonderici:%s\nAlici:%s\nDurum:%s\n",hash_tablo[index]->takipNo,hash_tablo[index]->gonderici,hash_tablo[index]->alici,hash_tablo[index]->kargoDurum);
            return;
        }
        index=double_hash_f(takipNo,deneme);
        deneme++;
    }
    printf("Takip No %d bulunamadi.\n",takipNo);
}
void kargoTeslimEt(int takipNo){
    int index=hash_f(takipNo);
    int deneme=1;
    while(hash_tablo[index]!=NULL){
        if(hash_tablo[index]->takipNo==takipNo){
            printf("Takip No %d hash tablosunda %d adimda bulundu ve teslim edildi.\n",takipNo,deneme);
            free(hash_tablo[index]);
            hash_tablo[index]=NULL;
            return;
        }
        index=double_hash_f(takipNo, deneme);
        deneme++;
    }
    printf("Takip No %d bulunamadi.\n",takipNo);
}
void kargolariListele(){
    printf("Hash Tablosundaki Kargolar:\n");
    for(int i=0;i<TABLO_BOYUTU;i++){
        if (hash_tablo[i] != NULL){
            printf("Index %d - Takip No: %d\n",i,hash_tablo[i]->takipNo);
            printf("Gonderici:%s\nAlici:%s\nDurum:%s\n",hash_tablo[i]->gonderici,hash_tablo[i]->alici,hash_tablo[i]->kargoDurum);
            printf("\n");
        }
    }
}

int main(){
    int islem;
    int takipNo;
    while (1) {
        printf("\n0 - Texten Veri Girisi Yap\n1 - Tum Kargolari Listele\n2 - Kargo Girisi\n3 - Kargo Takip\n4 - Kargoyu Teslim Et\n5 - Cikis\n\n");
        printf("Lutfen yapmak istediginiz islemi seciniz: ");
        scanf("%d", &islem);
        switch(islem){
            case 0:
                kargolariTopluEkle();
                break;
            case 1:
                kargolariListele();
                break;
            case 2:
                kargoGiris();
                break;
            case 3:
                printf("Kargo takip numarasini giriniz:");
                scanf("%d",&takipNo);
                kargoTakipDurumu(takipNo);
                break;
            case 4:
                printf("Kargo takip numarasini giriniz:");
                scanf("%d",&takipNo);
                kargoTeslimEt(takipNo);
                break;
            case 5:
                printf("Cikis yapiliyor...\n");
                exit(EXIT_SUCCESS);
            default:
                printf("Gecersiz islem.Lutfen tekrar deneyiniz.\n");
        }
    }
    return 0;
}
