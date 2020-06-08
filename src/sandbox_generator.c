#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <gtk/gtk.h>

#define len 128

char file_name[len];
bool SandboxReturnOne=false;
int flag=0;

void read_input(FILE* fp){
    char buff[len];
    char* str=NULL;
    char* value=NULL;
    if(fgets(buff,len,fp)){
        //printf("Read: %s\n",buff);
        str=strtok(buff," ");
        if(strcmp(str,"SandboxReturnOne")!=0){
            printf("Wrong input file format\n");
            fclose(fp);
            exit(-1);
        }
        str=strtok(NULL," ");
        value=strtok(str,"\n");
        if(strcmp(value,"true")==0){
            SandboxReturnOne=true;
        }
        else if(strcmp(value,"false")==0){
            SandboxReturnOne=false;
        }
        else{
            printf("Wrong input file format\n");
            fclose(fp);
            exit(-1);
        }
    }
}

void generate_header_file(){
    FILE* header=fopen("Sandbox_Cfg.h","w+");
    if(!header){
        perror("Sandbox_Cfg.h");
        exit(-1);
    }
    
    fputs("/**\n * \\file\n *\n * \\brief AUTOSAR Sandbox\n *\n * This file contains the generated configuration\n",header);
    fputs(" * of the AUTOSAR module Sandbox.\n *\n * The layout of this file must not change!\n */\n\n",header);
    fputs("/*====================================================================================================*/\n\n",header);
    fputs("/** \\brief AUTOSAR definition for 'off' */\n#define STD_OFF 0U\n\n/** \\brief AUTOSAR definition for 'on' */\n#define STD_ON  1U\n\n",header);
    fputs("/*==============================[generated]============================================================*/\n\n",header);
    fputs("/* !LINKSTO CUSTOMER_SANDBOX_0001_Conf, 1,\n */\n",header);
    fputs("/*Switch indication the output of a function\n     STD_ON if SandboxReturnOne is true\n",header);
    fputs("     STD_OFF if SandboxReturnOne is false */\n#define  SANDBOX_RETURN_ONE  ",header);
    if(SandboxReturnOne==true){
        fputs("STD_ON\n",header);
    }
    else{
        fputs("STD_OFF\n",header);
    }
    
    
    fclose(header);
}

void start_generator(){
	FILE* fp;
    
	if(file_name==NULL){
        printf("Please type a valid input file name!\n");
		return;
	}

    fp=fopen(file_name,"r");
    if(!fp){
        perror(file_name);
        printf("Please type a valid input file name!\n");
        return;
    }

   	read_input(fp);
    fclose(fp);
    printf("SandboxReturnOne=%s\n",SandboxReturnOne?"true":"false");
    generate_header_file();
    printf("Header file Sandbox_Cfg.h was created succesfully\n");
    flag=1;
}

void on_input_entry_changed(GtkEntry* e){
	snprintf(file_name,len,"%s",gtk_entry_get_text(e));
}

void on_generate_button_clicked(){
	start_generator();
	if(flag==1){
        gtk_main_quit();
    }
}

void on_main_window_destroy()
{
    gtk_main_quit();
}

int main(int argc, char* argv[])
{
	GObject *window;
	GObject *button;
	GObject *entry;
	GtkBuilder *builder;
	GError *err = NULL;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	if(gtk_builder_add_from_file(builder,"glade/generator_window.glade",&err)==0){
		g_printerr ("Error loading file: %s\n", err->message);
      	g_clear_error (&err);
      	return 1;
	}

	window = gtk_builder_get_object (builder, "main_window");
  	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  	entry = gtk_builder_get_object(builder,"input_entry");
  	g_signal_connect(entry,"changed",G_CALLBACK(on_input_entry_changed),NULL);

  	button = gtk_builder_get_object (builder, "generate_button");
  	g_signal_connect (button, "clicked", G_CALLBACK (on_generate_button_clicked), NULL);

  	

    g_object_unref(builder);

    gtk_widget_show((GtkWidget*)window);                
    gtk_main();

    //printf("File name is: %s and this is after\n",file_name);

	return 0;
}