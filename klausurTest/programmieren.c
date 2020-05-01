/*
 * programmieren.c
 *
 * Created: 01.05.2020 15:15:59
 *  Author: jensh
 */ 
 /*
 ein Taster, bei dem auf 0.1 ms genau abgefragt wird,
 in welchem Zeitabstan dieser Betätigt wurde.
 Abfrage möglich genau.
 Die Zeitangabe soll über den uart an die Konsole des PC (HTerm) gesendet werden.

 Zusatz:
 Der Tasterdruck wird nur gewertet, wenn währenddessen ein zweiter Taster
 (über polling abgefragt) gedrückt ist.
*/
