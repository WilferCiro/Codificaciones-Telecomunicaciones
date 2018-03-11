from __future__ import unicode_literals
import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, Gio, GLib
import threading
import serial # Librería para puerto Serial

class Gui():
	thread = None
	builder = Gtk.Builder()
	emisor_serial = serial.Serial()
	receptor_serial = serial.Serial();
	
	def ver_puertos(self):
		if self.emisor_serial.is_open and self.receptor_serial.is_open:			
			leido2=self.receptor_serial.readline() # leer la cadena que envía arduino
			leido2=leido2.decode()	
			x = leido2.split('--');		
			self.builder.get_object("DataRec").set_text("Codificated: "+x[0]+"\nReceived: "+x[1])
		else:
			self.builder.get_object("BitsLabel").set_text("Puerto no abierto")
			try:
				t = threading.Timer(2, self.ver_puertos)
				t.start()
			except:
				t = threading.Timer(1, self.ver_puertos)
				t.start()
	
	def connect_serial(self, *args):
		emisor = self.builder.get_object("SerialPortEmisor").get_active_text()
		receptor = self.builder.get_object("SerialPortReceptor").get_active_text()
		if (emisor == receptor):
			self.builder.get_object("BitsLabel").set_text("no pueden tener el mismo puerto serial")
		else:
			self.emisor_serial.port=emisor # Se establece el puerto de conexion con el del combobox
			self.emisor_serial.open() # Abre el puerto serial
			self.emisor_serial.baudrate=9600
			
			self.receptor_serial.port = receptor
			self.receptor_serial.open()
			self.receptor_serial.baudrate=9600
			
			if self.emisor_serial.is_open and self.receptor_serial.is_open: #Condiciín
				self.builder.get_object("BitsLabel").set_text("Puerto Conectado") # Poner el indicativo de que esta abierto el puerto en un label
			else:
				self.builder.get_object("BitsLabel").set_text("Puerto NO Conectado") # Poner el indicativo de que esta abierto el puerto en un label
	
	def envia_texto(self):
		texto = self.builder.get_object("WordSend").get_text()
		self.emisor_serial.write(texto.encode())
		leido=self.emisor_serial.readline() # leer la cadena que envía arduino
		leido=leido.decode()
		x = leido.split('--');
		self.builder.get_object("BitsLabel").set_text("Sent: "+x[0]+"\nCondificated: "+x[2]+"\nErrors in bits: "+x[1])
		self.builder.get_object("CodificateBtn").set_sensitive(True)
		
	def send_text(self, *args):
		if self.emisor_serial.is_open and self.receptor_serial.is_open:
			texto = self.builder.get_object("WordSend").get_text()
			if len(texto)==3:
				self.builder.get_object("BitsLabel").set_text("Sending, wait...");
				self.builder.get_object("DataRec").set_text("Receiving, wait...");
				self.builder.get_object("CodificateBtn").set_sensitive(False)
				self.thread = threading.Thread(
						target=self.envia_texto,
						args=()
				)
				self.thread.daemon = True
				self.thread.start()
				t = threading.Timer(2, self.ver_puertos)
				t.start()
			else:
				self.builder.get_object("BitsLabel").set_text("Error, word must be 3 characters");	
		else:
			self.builder.get_object("BitsLabel").set_text("Puerto serial no conectado");
			
	def cierraVentana(self, *args):
		if self.emisor_serial.is_open and self.receptor_serial.is_open:			
			self.emisor_serial.close()
			self.receptor_serial.close()
		Gtk.main_quit(*args)
	
	def __init__(self):
		#accion_actual
		self.builder.add_from_file("visual/interfaz.glade")
				
		#Ventana Principal
		_window = self.builder.get_object("PrincipalWindow")
		_window.connect("delete_event", self.cierraVentana)
		_window.set_default_size(930, 330)
		_window.set_position(Gtk.WindowPosition.CENTER)
		
		self.builder.get_object("CodificateBtn").connect("clicked", self.send_text)
		self.builder.get_object("SerialBtn").connect("clicked", self.connect_serial)
	
		self.builder.get_object("CodificationKind").set_text("Codification: Viterbi");
		self.builder.get_object("Title").set_text("Canal Codification: Viterbi");
		
		self.builder.get_object("WordSend").set_text("ITU")
		self.builder.get_object("wordLabel").set_text("Word (3 characters)")
		
		
		_window.show_all()
		Gtk.main()

def start():
	print("Aplicación Iniciada")
	gui = Gui()

if __name__ == "__main__":
	start()

