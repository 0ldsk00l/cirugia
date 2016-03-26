/*
 * Cirugía - Copyright (C) R. Danbrook 2015-2016
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the program nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <gtk/gtk.h>

#include "cirugia.h"
#include "header.h"
#include "ips.h"
#include "rom.h"

GtkBuilder *builder;
GtkWidget *mainwindow;
GtkWidget *statusbar;

GtkWidget *spinbuttonmapper;
GtkWidget *spinbuttonsubmapper;

GtkWidget *spinbuttonprgrom;
GtkWidget *spinbuttonprgram;
GtkWidget *spinbuttonprgnvram;

GtkWidget *spinbuttonchrrom;
GtkWidget *spinbuttonchrram;
GtkWidget *spinbuttonchrnvram;

GtkWidget *filesaveas;
GtkWidget *fileapplyips;
GtkWidget *filecreateips;

GtkWidget *radiobutton1;
GtkWidget *radiobutton2;
GtkWidget *radiobuttonh;
GtkWidget *radiobuttonv;
GtkWidget *radiobuttonf;
GtkWidget *radiobuttonntsc;
GtkWidget *radiobuttonpal;
GtkWidget *radiobuttonboth;
GtkWidget *radiobuttonhomeconsole;
GtkWidget *radiobuttonvssystem;
GtkWidget *radiobuttonpc10;

GtkWidget *comboboxvsppu;
GtkWidget *comboboxvsmode;

GtkWidget *checkbuttontrainer;

int romloaded = 0;

extern int version;

int cir_gtk_header_parse() {
	// Check the ROM header information
	
	if (version == 2) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton2), TRUE);
	}
	else {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobutton1), TRUE);
	}
	
	// Get the mapper number
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonmapper), cir_header_get_mapper());
	
	// Get the submapper number (NES 2.0)
	if (version == 2) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonsubmapper), cir_header_get_submapper());
	}
	
	// Get the PRG ROM size
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonprgrom), cir_header_get_prgrom());
	
	// Check if PRG RAM or battery is present
	if (cir_header_get_prgram_present()) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonprgram), cir_header_get_prgram());
		
		if (version == 2) {
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonprgnvram), cir_header_get_prgnvram());
		}
	}
	
	// Check the CHR ROM size or detect CHR RAM
	if (cir_header_get_chrrom()) {
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonchrrom), cir_header_get_chrrom());
	}
	else {
		if (version == 2) {
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonchrram), cir_header_get_chrram());
			gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonchrnvram), cir_header_get_chrnvram());
		}
		else { gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonchrram), cir_header_get_chrram()); }
	}
	
	// Check the mirroring
	int mirroring = cir_header_get_mirroring();
	
	if (mirroring == 2) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonf), TRUE);
	}
	else if (mirroring == 1) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonv), TRUE);
	}
	else {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonh), TRUE);
	}
	
	// TV System
	int tvsystem = cir_header_get_tvsystem();
	if (tvsystem == 0) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonntsc), TRUE);
	}
	else if (tvsystem == 1) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonpal), TRUE);
	}
	else if (tvsystem == 2) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonboth), TRUE);
	}
	
	// Check if there's a trainer
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbuttontrainer), cir_header_get_trainer());
	
	int system = cir_header_get_system();
	if (system == 2) { // PC-10
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonpc10), TRUE);
	}
	else if (system == 1) { // Vs. System
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonvssystem), TRUE);
		
		// Detect VS. System hardware if it's NES 2.0
		if (version == 2) {
			// Check the VS. System's PPU hardware
			gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxvsppu), cir_header_get_vsppu());
			
			// Check the VS. System's PPU Mode
			gtk_combo_box_set_active(GTK_COMBO_BOX(comboboxvsmode), cir_header_get_vsmode());
		}
	}
	else { // Home Console
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radiobuttonhomeconsole), TRUE);
	}
	
	return 1;
}

void cir_gtk_reset_spinbuttons() {
	// Set the spin buttons to default values
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonmapper), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonsubmapper), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonprgrom), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonprgram), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonprgnvram), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonchrrom), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonchrram), 0);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinbuttonchrnvram), 0);
}

void cir_gtk_set_sensitive() {
	// Set the GUI elements sensitive or insensitive
	gtk_widget_set_sensitive(filesaveas, TRUE);
	gtk_widget_set_sensitive(fileapplyips, TRUE);
	gtk_widget_set_sensitive(filecreateips, TRUE);
	
	gtk_widget_set_sensitive(radiobutton1, TRUE);
	gtk_widget_set_sensitive(radiobutton2, TRUE);
	
	gtk_widget_set_sensitive(spinbuttonmapper, TRUE);
	// NES 2.0 only
	gtk_widget_set_sensitive(spinbuttonsubmapper, version == 2 ? TRUE : FALSE);
	
	gtk_widget_set_sensitive(spinbuttonprgrom, TRUE);
	gtk_widget_set_sensitive(spinbuttonprgram, TRUE);
	gtk_widget_set_sensitive(spinbuttonprgnvram, TRUE);
	
	gtk_widget_set_sensitive(spinbuttonchrrom, TRUE);
	gtk_widget_set_sensitive(spinbuttonchrram, version == 2 ? TRUE : FALSE);
	gtk_widget_set_sensitive(spinbuttonchrnvram, version == 2 ? TRUE : FALSE);
	
	gtk_widget_set_sensitive(radiobuttonh, TRUE);
	gtk_widget_set_sensitive(radiobuttonv, TRUE);
	gtk_widget_set_sensitive(radiobuttonf, TRUE);
	
	// NES 2.0 only
	gtk_widget_set_sensitive(radiobuttonntsc, version == 2 ? TRUE : FALSE);
	gtk_widget_set_sensitive(radiobuttonpal, version == 2 ? TRUE : FALSE);
	gtk_widget_set_sensitive(radiobuttonboth, version == 2 ? TRUE : FALSE);
	
	gtk_widget_set_sensitive(radiobuttonhomeconsole, TRUE);
	gtk_widget_set_sensitive(radiobuttonvssystem, TRUE);
	// NES 2.0 only
	gtk_widget_set_sensitive(radiobuttonpc10, version == 2 ? TRUE : FALSE);
	
	// NES 2.0 only
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobuttonvssystem))) {
		gtk_widget_set_sensitive(comboboxvsppu, version == 2 ? TRUE : FALSE);
		gtk_widget_set_sensitive(comboboxvsmode, version == 2 ? TRUE : FALSE);
	}
	else {
		gtk_widget_set_sensitive(comboboxvsppu, FALSE);
		gtk_widget_set_sensitive(comboboxvsmode, FALSE);
	}
	
	gtk_widget_set_sensitive(checkbuttontrainer, TRUE);
}

void cir_gtk_message(const char* message) {    
	GtkWidget *messagewindow = gtk_message_dialog_new(
				GTK_WINDOW(mainwindow),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_INFO,
				GTK_BUTTONS_OK,
				message);
	gtk_dialog_run(GTK_DIALOG(messagewindow));
	gtk_widget_destroy(messagewindow);
}

void cir_gtk_status_update(char *message) {
	gtk_statusbar_remove_all(GTK_STATUSBAR(statusbar), 0);
	gtk_statusbar_push(GTK_STATUSBAR(statusbar), 0, message);
}

void cir_gtk_set_vsppu(GtkComboBox *combobox, gpointer data) {
	cir_header_set_vsppu(gtk_combo_box_get_active(combobox));
}

void cir_gtk_set_vsmode(GtkComboBox *combobox, gpointer data) {
	cir_header_set_vsmode(gtk_combo_box_get_active(combobox));
}

void cir_gtk_toggle_headerver(GtkWidget *radiobutton, gpointer data) {
	char label[16];
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton))) {
		snprintf(label, sizeof(label), "%s", gtk_button_get_label(GTK_BUTTON(radiobutton)));
	}
	
	if (!strncmp(label, "iNES", sizeof(label))) {
		cir_header_set_version(1);
		version = 1;
	}
	if (!strncmp(label, "NES 2.0", sizeof(label))) {
		cir_header_set_version(2);
		version = 2;
	}
	
	cir_gtk_set_sensitive();
}

void cir_gtk_set_mapper(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_mapper((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_set_submapper(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_submapper((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_set_prgrom(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_prgrom((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_set_prgram(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_prgram((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_set_prgnvram(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_prgnvram((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_set_chrrom(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_chrrom((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_set_chrram(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_chrram((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_set_chrnvram(GtkAdjustment *adjustment, gpointer data) {
	cir_header_set_chrnvram((int)gtk_adjustment_get_value(adjustment));
}

void cir_gtk_toggle_mirroring(GtkWidget *radiobutton, gpointer data) {
	char label[16];
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton))) {
		snprintf(label, sizeof(label), "%s", gtk_button_get_label(GTK_BUTTON(radiobutton)));
	}
	
	if (!strncmp(label, "Horizontal", sizeof(label))) {
		cir_header_set_mirroring(0);
	}
	if (!strncmp(label, "Vertical", sizeof(label))) {
		cir_header_set_mirroring(1);
	}
	if (!strncmp(label, "Four Screen", sizeof(label))) {
		cir_header_set_mirroring(2);
	}
}

void cir_gtk_toggle_system(GtkWidget *radiobutton, gpointer data) {
	char label[16];
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton))) {
		snprintf(label, sizeof(label), "%s", gtk_button_get_label(GTK_BUTTON(radiobutton)));
	}
	
	if (!strncmp(label, "Home Console", sizeof(label))) {
		cir_header_set_system(0);
	}
	if (!strncmp(label, "Vs. System", sizeof(label))) {
		cir_header_set_system(1);
	}
	if (!strncmp(label, "PlayChoice-10", sizeof(label))) {
		cir_header_set_system(2);
	}
	cir_gtk_set_sensitive();
}

void cir_gtk_toggle_tvsystem(GtkWidget *radiobutton, gpointer data) {
	char label[16];
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radiobutton))) {
		snprintf(label, sizeof(label), "%s", gtk_button_get_label(GTK_BUTTON(radiobutton)));
	}
	
	if (!strncmp(label, "NTSC", sizeof(label))) {
		cir_header_set_tvsystem(0);
	}
	if (!strncmp(label, "PAL", sizeof(label))) {
		cir_header_set_tvsystem(1);
	}
	if (!strncmp(label, "NTSC/PAL", sizeof(label))) {
		cir_header_set_tvsystem(2);
	}
}

void cir_gtk_toggle_trainer(GtkToggleButton *togglebutton, gpointer data) {
	cir_header_set_trainer(gtk_toggle_button_get_active(togglebutton));
}

void cir_gtk_help_about(GtkWidget *widget, gpointer data) {
	gtk_window_set_transient_for(GTK_WINDOW(data), GTK_WINDOW(mainwindow));
	gtk_dialog_run(data);
	gtk_widget_hide(data);
}

void cir_gtk_file_save() {
	// Save a file using a GTK+ dialog
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Save ROM (.nes)",
				GTK_WINDOW(mainwindow),
				GTK_FILE_CHOOSER_ACTION_SAVE,
				("_Cancel"), GTK_RESPONSE_CANCEL,
				("_Save"), GTK_RESPONSE_ACCEPT,
				NULL);
	
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "edited.nes");
	
	if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		
		if (romloaded) {
			char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
			cir_rom_write(filename);
			char message[128];
			snprintf(message, sizeof(message), "Saved: %s", filename);
			cir_gtk_status_update(message);
			g_free(filename);
		}
	}
	
	gtk_widget_destroy(dialog);
}

void cir_gtk_file_open() {
	// Open a file using a GTK+ dialog
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
				"Select a ROM",
				GTK_WINDOW(mainwindow),
				GTK_FILE_CHOOSER_ACTION_OPEN,
				("_Cancel"), GTK_RESPONSE_CANCEL,
				("_Open"), GTK_RESPONSE_ACCEPT,
				NULL);
	
	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "NES ROMs");
	gtk_file_filter_add_pattern(filter, "*.nes");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		gtk_widget_destroy(dialog);
		
		cir_rom_cleanup(); romloaded = 0;
		
		if (cir_rom_load(filename)) {
			
			cir_gtk_reset_spinbuttons();
			
			// Split the header and the ROM
			cir_rom_split_header_rom();
			
			if (cir_header_validate()) {
				// Check header version
				version = cir_header_get_version();
				cir_gtk_header_parse();
				char message[128];
				snprintf(message, sizeof(message), "Loaded: %s", filename);
				cir_gtk_status_update(message);
				cir_gtk_set_sensitive();
				romloaded = 1;
			}
			else { cir_gtk_message("Invalid File (No NES ROM Header)"); }
		}
		
		g_free(filename);
	}
	else { gtk_widget_destroy(dialog); }
}

void cir_gtk_file_patchrom() {
	// Save a file using a GTK+ dialog
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Save Patched ROM (.nes)",
				GTK_WINDOW(mainwindow),
				GTK_FILE_CHOOSER_ACTION_SAVE,
				("_Cancel"), GTK_RESPONSE_CANCEL,
				("_Save"), GTK_RESPONSE_ACCEPT,
				NULL);
	
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "patched.nes");
	
	if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		cir_ips_rom_write(filename);
		g_free(filename);
	}
	
	gtk_widget_destroy(dialog);
}

void cir_gtk_file_applyips() {
	// Open a file using a GTK+ dialog
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
				"Select IPS Patch",
				GTK_WINDOW(mainwindow),
				GTK_FILE_CHOOSER_ACTION_OPEN,
				("_Cancel"), GTK_RESPONSE_CANCEL,
				("_Open"), GTK_RESPONSE_ACCEPT,
				NULL);
	
	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "IPS patches");
	gtk_file_filter_add_pattern(filter, "*.ips");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		gtk_widget_destroy(dialog);
		
		if (cir_ips_load(filename)) {
			// Parse the IPS patch without applying
			cir_ips_parse(0);
			cir_gtk_file_patchrom();
			char message[128];
			snprintf(message, sizeof(message), "Applied Patch: %s", filename);
			cir_gtk_status_update(message);
		}
		else { cir_gtk_message("Invalid Patch File"); }
		
		g_free(filename);
	}
	else { gtk_widget_destroy(dialog); }
}

void cir_gtk_file_ips() {
	// Save a file using a GTK+ dialog
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Save IPS Patch (.ips)",
				GTK_WINDOW(mainwindow),
				GTK_FILE_CHOOSER_ACTION_SAVE,
				("_Cancel"), GTK_RESPONSE_CANCEL,
				("_Save"), GTK_RESPONSE_ACCEPT,
				NULL);
	
	gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "patch.ips");
	
	if (gtk_dialog_run(GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		cir_ips_write(filename);
		g_free(filename);
	}
	
	gtk_widget_destroy(dialog);
}

void cir_gtk_file_createips() {
	// Open a file using a GTK+ dialog
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
				"Select Modified ROM",
				GTK_WINDOW(mainwindow),
				GTK_FILE_CHOOSER_ACTION_OPEN,
				("_Cancel"), GTK_RESPONSE_CANCEL,
				("_Open"), GTK_RESPONSE_ACCEPT,
				NULL);
	
	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_set_name(filter, "NES ROMs");
	gtk_file_filter_add_pattern(filter, "*.nes");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
	
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		char *filename;
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		gtk_widget_destroy(dialog);
		
		if (cir_rom_load_diff(filename)) {
			cir_ips_diff();
			cir_gtk_file_ips();
		}
		else { cir_gtk_message("Invalid NES ROM"); }
		
		g_free(filename);
	}
	else { gtk_widget_destroy(dialog); }
}

int main(int argc, char* argv[]) {
	// The main function
	
	GError *error = NULL;
	
	// Initialize GTK+
	gtk_init(&argc, &argv);
	
	// Create new GtkBuilder
	builder = gtk_builder_new();
	
	// Load UI from a file
	if (!gtk_builder_add_from_file(builder, "gtk/cirugia.ui", &error)) {
		g_warning("%s", error->message);
		g_free(error);
		return(1);
	}
	
	// Get pointers from UI
	mainwindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainwindow"));
	statusbar = GTK_WIDGET(gtk_builder_get_object(builder, "statusbar"));
	
	filesaveas = GTK_WIDGET(gtk_builder_get_object(builder, "filesaveas"));
	fileapplyips = GTK_WIDGET(gtk_builder_get_object(builder, "fileapplyips"));
	filecreateips = GTK_WIDGET(gtk_builder_get_object(builder, "filecreateips"));
	
	radiobutton1 = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton1"));
	radiobutton2 = GTK_WIDGET(gtk_builder_get_object(builder, "radiobutton2"));
	
	spinbuttonmapper = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonmapper"));
	spinbuttonsubmapper = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonsubmapper"));
	
	spinbuttonprgrom = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonprgrom"));
	spinbuttonprgram = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonprgram"));
	spinbuttonprgnvram = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonprgnvram"));
	
	spinbuttonchrrom = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonchrrom"));
	spinbuttonchrram = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonchrram"));
	spinbuttonchrnvram = GTK_WIDGET(gtk_builder_get_object(builder, "spinbuttonchrnvram"));
	
	radiobuttonh = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonh"));
	radiobuttonv = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonv"));
	radiobuttonf = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonf"));
	
	radiobuttonntsc = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonntsc"));
	radiobuttonpal = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonpal"));
	radiobuttonboth = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonboth"));
	
	radiobuttonhomeconsole = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonhomeconsole"));
	radiobuttonvssystem = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonvssystem"));
	radiobuttonpc10 = GTK_WIDGET(gtk_builder_get_object(builder, "radiobuttonpc10"));
	
	comboboxvsppu = GTK_WIDGET(gtk_builder_get_object(builder, "comboboxvsppu"));
	comboboxvsmode = GTK_WIDGET(gtk_builder_get_object(builder, "comboboxvsmode"));
	
	checkbuttontrainer = GTK_WIDGET(gtk_builder_get_object(builder, "checkbuttontrainer"));
	
	// Connect signals
	gtk_builder_connect_signals(builder, NULL);
	
	// Destroy builder
	g_object_unref(G_OBJECT(builder));
	
	// Show the window
	gtk_widget_show(mainwindow);
	
	// Start GTK+ main loop
	gtk_main();
	
	// Do cleanups
	if (romloaded) { cir_rom_cleanup(); }
	
	return 0;
}
