#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* DECLARCION DE STRUCTS */
struct Entrada;
struct Visitante;
struct Zona;
struct Atraccion;
struct NodoVisitantes;
struct NodoEntradas;
struct NodoZonas;
struct NodoAtraccion;
struct NodoFila;
struct Parque;

/* VARIABLES GLOBALES */
int ENTRADA_GENERAL = 0;
int ENTRADA_INFANTIL = 0;
int ENTRADA_FAMILIAR = 0;
int ENTRADA_PRIORITARIA = 0;

/* DECLARACION DE FUNCIONES */
void limpiar_pantalla(void);
char *copiar_string(const char *);
struct Tiempo obtener_hora_actual(void);

void ejecutar_submenu_entradas(struct NodoEntradas **);
void ejecutar_submenu_visitantes(struct Parque *);
void ejecutar_submenu_filas(struct Parque *);
void ejecutar_submenu_zonas(struct NodoZonas **);
void ejecutar_submenu_atracciones(struct NodoZonas *);

int escoger_opcion(void);
int menu_inicializar_parque(struct Parque *);
void mostrar_menu_principal(void);
void mostrar_submenu_entradas(void);
void menu_comprar_entrada(struct NodoEntradas **);
void menu_cambiar_estado_entrada(struct NodoEntradas **);
void menu_eliminar_entrada(struct NodoEntradas **);
void mostrar_submenu_visitantes(void);
void menu_agregar_visitante(struct Parque *);
void menu_eliminar_visitante(struct NodoVisitantes **);
void mostrar_submenu_filas(void);
void menu_agregar_grupo_fila(struct Parque *);
void menu_avanzar_fila_atraccion(struct NodoZonas *);
void mostrar_submenu_zonas(void);
void menu_agregar_zona_al_parque(struct NodoZonas **);
void menu_eliminar_zona_del_parque(struct NodoZonas **);
void menu_agregar_o_remover_visitante_zona(struct NodoZonas *);
void mostrar_submenu_atracciones(void);
void menu_agregar_atraccion_zona(struct NodoZonas *);
void menu_eliminar_atraccion_zona(struct NodoZonas *);
void menu_mover_atraccion_distinta_zona(struct NodoZonas *);
void menu_modificar_estado_atraccion(struct NodoZonas *);

struct Zona *obtener_zona_por_id(struct NodoZonas *, int);
int agregar_zona(struct NodoZonas **, const char *, const char *, int, int, int, int, int, int);
int eliminar_zona(struct NodoZonas **, int);
int agregar_o_remover_visitantes_zona(struct NodoZonas *, int, int);

struct Visitante* buscar_visitante_por_id(struct NodoVisitantes *, int);
int total_personas_diario_parque(struct Parque *);
int total_personas_dentro_parque(struct Parque *);
int agregar_visitante(struct Parque *, struct Entrada *, char *, char *, int, float);
int eliminar_visitante(struct NodoVisitantes **, int);

int agregar_grupo_fila(struct NodoVisitantes *, struct Atraccion *, int *, int, int);
int vaciar_filas_atraccion(struct NodoZonas *, int);
int avanzar_fila_atraccion(struct NodoZonas *, int);

void ver_filas_atracciones(struct NodoZonas *);
void ver_entradas_compradas(struct NodoEntradas *);
void ver_visitantes_parque(struct NodoVisitantes *);
void ver_zonas_actuales(struct NodoZonas *);
void ver_atracciones_actuales(struct NodoZonas *);
void ver_reporte_atracciones_actual(struct NodoZonas *);
void ver_reporte_general_dia(struct Parque *);

struct Entrada *buscar_entrada_por_id(struct NodoEntradas *, int);
int calcular_recaudacion_entradas(struct Parque *);
int comprar_entrada(struct NodoEntradas **, char *, int);
int cambiar_estado_entrada(struct NodoEntradas **, int, char *);
int eliminar_entrada(struct NodoEntradas **, int);

struct Atraccion *buscar_atraccion_por_id(struct NodoZonas *, int);
struct Atraccion *obtener_atraccion_mayor_pico(struct Parque *);
struct Atraccion *obtener_atraccion_mas_visitada(struct Parque *);
int agregar_atraccion(struct NodoZonas *, struct Zona *, const char *, const char *, int, int, int, float);
int eliminar_atraccion(struct NodoZonas *, int);
int mover_atraccion(struct NodoZonas *, struct Zona *, int);
int cambiar_estado_atraccion(struct NodoZonas *, int, const char *);

/* ========================================================================== */
/* 2. ESTRUCTURAS INDEPENDIENTES Y HOJA */
/* ========================================================================== */

struct Tiempo {
    int hora;
    int minutos;
};

struct Entrada {
    int id;
    char *tipo; /* ["general", "infantil", "familiar", "vip"] */
    int valor;
    char *estado; /* ["activa", "utilizada", "vencida", "anulada"] */
};

struct NodoFila {
    int ids_grupo[10]; /* ids_grupo[0] es el representante del grupo, la id del visitante */
    int tam_grupo;
    struct NodoFila *sig;
};

struct Fila {
    struct NodoFila *frente;
    struct NodoFila *final;
};

struct Visitante {
    int id;
    char *nombre;
    char rut[11];
    int edad;
    float altura;
    struct Entrada *entrada;
};

struct Zona {
    int id;
    char *nombre;
    char *tematica;

    int cap_max;
    int visitantes_actuales;

    struct Tiempo hora_apertura;
    struct Tiempo hora_cierre;

    int atracciones_max;
    struct NodoAtraccion *head_atracciones;
};

struct Atraccion {
    int id;
    char *nombre;
    char *estado; /* ["operativa", "mantenimiento", "cerrada", "fuera_de_servicio"] */
    char *tematica;
    int duracion; /* En minutos */
    int cap_max; /* Capacidad por ciclo */

    int edad_min;
    float altura_min;

    struct Fila cola_general;
    struct Fila cola_prioritaria;

    int visitantes_totales;
    int pico_cola_general;
    int pico_cola_prioritaria;
};

struct NodoVisitantes {
    struct Visitante *datos;
    struct NodoVisitantes *izq, *der;
};

struct NodoEntradas {
    struct Entrada *entrada;
    struct NodoEntradas *sig;
};

struct NodoZonas {
    struct Zona *datos;
    struct NodoZonas *sig;
};

struct NodoAtraccion {
    struct Atraccion *datos;
    struct NodoAtraccion *sig;
};

struct Parque {
    int cap_max;
    struct NodoVisitantes *raiz_visitantes; /* árbol binario ordenado por id del visitante */
    struct NodoEntradas *head_entradas; /* Simplemente enlazada */
    struct NodoZonas *head_zonas; /* Simplemente enlazada */
};

/*====================================
----------------UTILS-----------------
======================================*/

void limpiar_pantalla(void) {
    printf("\033[2J\033[3J\033[H");
    fflush(stdout);
}

char *copiar_string(const char *origen) {
    char *destino;
    size_t tamano;

    if (origen == NULL) {
        return NULL;
    }

    tamano = strlen(origen) + 1;

    destino = (char *)malloc(tamano);

    if (destino != NULL) {
        strcpy(destino, origen);
    }

    return destino;
}

struct Tiempo obtener_hora_actual(void) {
    time_t tiempo_segundos;
    struct tm *tiempo_local;
    struct Tiempo hora_sistema;

    tiempo_segundos = time(NULL);

    tiempo_local = localtime(&tiempo_segundos);

    if (tiempo_local != NULL) {
        hora_sistema.hora = tiempo_local->tm_hour;
        hora_sistema.minutos = tiempo_local->tm_min;
    } else {
        hora_sistema.hora = 0;
        hora_sistema.minutos = 0;
    }

    return hora_sistema;
}

/*====================================
-------------ATRACCIONES--------------
======================================*/

struct Atraccion *buscar_atraccion_por_id(struct NodoZonas *head_zonas, int id_atraccion) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion *atraccion_actual;

    if (head_zonas == NULL) {
        return NULL;
    }

    zona_actual = head_zonas;

    while (zona_actual != NULL) {
        if (zona_actual->datos != NULL) {

            atraccion_actual = zona_actual->datos->head_atracciones;
            while (atraccion_actual != NULL) {

                if (atraccion_actual->datos != NULL && atraccion_actual->datos->id == id_atraccion)
                    return atraccion_actual->datos;
                atraccion_actual = atraccion_actual->sig;
            }
        }
        zona_actual = zona_actual->sig;
    }

    return NULL;
}

struct Atraccion *obtener_atraccion_mayor_pico(struct Parque *parque) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion *atraccion_actual;
    struct Atraccion *atraccion_max;
    int max_pico;
    int suma_picos;

    if (parque == NULL || parque->head_zonas == NULL) {
        return NULL;
    }

    max_pico = -1;
    atraccion_max = NULL;

    zona_actual = parque->head_zonas;
    while (zona_actual != NULL) {

        if (zona_actual->datos != NULL) {

            atraccion_actual = zona_actual->datos->head_atracciones;
            while (atraccion_actual != NULL) {

                if (atraccion_actual->datos != NULL) {
                    suma_picos = atraccion_actual->datos->pico_cola_general +
                                 atraccion_actual->datos->pico_cola_prioritaria;

                    if (suma_picos > max_pico) {
                        max_pico = suma_picos;
                        atraccion_max = atraccion_actual->datos;
                    }
                }

                atraccion_actual = atraccion_actual->sig;
            }
        }

        zona_actual = zona_actual->sig;
    }

    return atraccion_max;
}

struct Atraccion *obtener_atraccion_mas_visitada(struct Parque *parque) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion *atraccion_actual;
    struct Atraccion *atraccion_max;
    int max_visitantes;

    if (parque == NULL || parque->head_zonas == NULL) {
        return NULL;
    }

    max_visitantes = -1;
    atraccion_max = NULL;

    zona_actual = parque->head_zonas;
    while (zona_actual != NULL) {

        if (zona_actual->datos != NULL) {

            atraccion_actual = zona_actual->datos->head_atracciones;
            while (atraccion_actual != NULL) {

                if (atraccion_actual->datos != NULL) {

                    if (atraccion_actual->datos->visitantes_totales > max_visitantes) {
                        max_visitantes = atraccion_actual->datos->visitantes_totales;
                        atraccion_max = atraccion_actual->datos;
                    }
                }

                atraccion_actual = atraccion_actual->sig;
            }
        }

        zona_actual = zona_actual->sig;
    }

    return atraccion_max;
}

int agregar_atraccion(struct NodoZonas *head_zonas, struct Zona *zona, const char *nombre,
                      const char *tematica, int duracion, int cap_max, int edad_min, float altura_min) {

    int contador_atracciones;
    int nueva_id;
    int id_ocupada;
    struct NodoZonas *zona_aux;
    struct NodoAtraccion *actual;
    struct NodoAtraccion *nuevo_nodo;
    struct Atraccion *nueva_atraccion;

    if (head_zonas == NULL || zona == NULL) {
        return -1; /* PUNTERO NULO */
    }

    if (zona->tematica == NULL || tematica == NULL || strcmp(zona->tematica, tematica) != 0) {
        return -2; /* TEMATICA INCOMPATIBLE */
    }

    contador_atracciones = 0;
    actual = zona->head_atracciones;
    while (actual != NULL) {
        contador_atracciones++;
        actual = actual->sig;
    }

    if (contador_atracciones >= zona->atracciones_max) {
        return -3; /*SIN ESPACIO*/
    }

    nueva_id = 1;
    id_ocupada = 1;

    while (id_ocupada) {
        id_ocupada = 0;
        zona_aux = head_zonas;

        while (zona_aux != NULL) {
            if (zona_aux->datos != NULL) {
                actual = zona_aux->datos->head_atracciones;

                while (actual != NULL) {
                    if (actual->datos != NULL && actual->datos->id == nueva_id) {
                        id_ocupada = 1;
                        break;
                    }
                    actual = actual->sig;
                }
            }

            if (id_ocupada) {
                break;
            }
            zona_aux = zona_aux->sig;
        }

        if (id_ocupada) {
            nueva_id++;
        }
    }

    nuevo_nodo = (struct NodoAtraccion *)malloc(sizeof(struct NodoAtraccion));
    nueva_atraccion = (struct Atraccion *)malloc(sizeof(struct Atraccion));

    if (nuevo_nodo == NULL || nueva_atraccion == NULL) {
        free(nuevo_nodo);
        free(nueva_atraccion);
        return -4; /* ERROR MEMORIA */
    }

    nueva_atraccion->nombre = copiar_string(nombre);
    nueva_atraccion->tematica = copiar_string(tematica);
    nueva_atraccion->estado = copiar_string("operativa");

    if (nueva_atraccion->nombre == NULL || nueva_atraccion->tematica == NULL || nueva_atraccion->estado == NULL) {
        free(nueva_atraccion->nombre);
        free(nueva_atraccion->tematica);
        free(nueva_atraccion->estado);
        free(nueva_atraccion);
        free(nuevo_nodo);
        return -4; /* ERROR MEMORIA */
    }

    nueva_atraccion->id = nueva_id;
    nueva_atraccion->duracion = duracion;
    nueva_atraccion->cap_max = cap_max;
    nueva_atraccion->edad_min = edad_min;
    nueva_atraccion->altura_min = altura_min;

    nueva_atraccion->cola_general.frente = NULL;
    nueva_atraccion->cola_general.final = NULL;
    nueva_atraccion->cola_prioritaria.frente = NULL;
    nueva_atraccion->cola_prioritaria.final = NULL;

    nueva_atraccion->visitantes_totales = 0;
    nueva_atraccion->pico_cola_general = 0;
    nueva_atraccion->pico_cola_prioritaria = 0;

    nuevo_nodo->datos = nueva_atraccion;
    nuevo_nodo->sig = zona->head_atracciones;
    zona->head_atracciones = nuevo_nodo;

    return 0; /* EXITO */
}

int eliminar_atraccion(struct NodoZonas *head_zonas, int id_atraccion) {
    struct NodoZonas *zona_actual;
    struct NodoAtraccion **enlace;
    struct NodoAtraccion *a_eliminar;
    struct NodoFila *aux_fila;
    struct NodoFila *sig_fila;

    if (head_zonas == NULL) {
        return -1; /* ZONA NULA */
    }

    zona_actual = head_zonas;

    while (zona_actual != NULL) {
        if (zona_actual->datos != NULL) {

            enlace = &zona_actual->datos->head_atracciones;

            while (*enlace != NULL) {
                if ((*enlace)->datos != NULL && (*enlace)->datos->id == id_atraccion) {

                    a_eliminar = *enlace;
                    *enlace = (*enlace)->sig;

                    if (a_eliminar->datos != NULL) {

                        aux_fila = a_eliminar->datos->cola_general.frente;
                        while (aux_fila != NULL) {
                            sig_fila = aux_fila->sig;
                            free(aux_fila);
                            aux_fila = sig_fila;
                        }

                        aux_fila = a_eliminar->datos->cola_prioritaria.frente;
                        while (aux_fila != NULL) {
                            sig_fila = aux_fila->sig;
                            free(aux_fila);
                            aux_fila = sig_fila;
                        }

                        free(a_eliminar->datos->nombre);
                        free(a_eliminar->datos->tematica);
                        free(a_eliminar->datos->estado);

                        free(a_eliminar->datos);
                    }

                    free(a_eliminar);

                    return 0;
                }

                enlace = &((*enlace)->sig);
            }
        }
        zona_actual = zona_actual->sig;
    }

    return -2; /* ATRACCION NO ENCONTRADA */
}

int mover_atraccion(struct NodoZonas *head_zonas, struct Zona *zona_objetivo, int id_atraccion) {
    int contador_atracciones;
    struct NodoZonas *zona_actual;
    struct Zona *zona_inicio;
    struct NodoAtraccion *actual_atr;
    struct NodoAtraccion **enlace;
    struct NodoAtraccion *nodo_a_mover;

    if (head_zonas == NULL || zona_objetivo == NULL) {
        return -1; /* ERR_PUNTERO_NULO */
    }

    zona_inicio = NULL;
    zona_actual = head_zonas;

    while (zona_actual != NULL && zona_inicio == NULL) {
        if (zona_actual->datos != NULL) {
            actual_atr = zona_actual->datos->head_atracciones;

            while (actual_atr != NULL) {
                if (actual_atr->datos != NULL && actual_atr->datos->id == id_atraccion) {
                    zona_inicio = zona_actual->datos;
                    break;
                }
                actual_atr = actual_atr->sig;
            }
        }
        zona_actual = zona_actual->sig;
    }

    if (zona_inicio == NULL) {
        return -4; /* ERR_ATRACCION_NO_ENCONTRADA */
    }

    if (zona_inicio == zona_objetivo) {
        return 0; /* EXITO */
    }

    if (zona_inicio->tematica == NULL || zona_objetivo->tematica == NULL ||
        strcmp(zona_inicio->tematica, zona_objetivo->tematica) != 0) {
        return -2; /* ERR_TEMATICA_INCOMPATIBLE */
    }

    contador_atracciones = 0;
    actual_atr = zona_objetivo->head_atracciones;
    while (actual_atr != NULL) {
        contador_atracciones++;
        actual_atr = actual_atr->sig;
    }

    if (contador_atracciones >= zona_objetivo->atracciones_max) {
        return -3; /* CAPACIDAD MAX */
    }

    enlace = &zona_inicio->head_atracciones;
    nodo_a_mover = NULL;

    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL && (*enlace)->datos->id == id_atraccion) {
            nodo_a_mover = *enlace;
            *enlace = (*enlace)->sig;
            break;
        }
        enlace = &((*enlace)->sig);
    }

    if (nodo_a_mover != NULL) {
        nodo_a_mover->sig = zona_objetivo->head_atracciones;
        zona_objetivo->head_atracciones = nodo_a_mover;
    }

    return 0; /* EXITO */
}

int cambiar_estado_atraccion(struct NodoZonas *head_zonas, int id_atraccion, const char *nuevo_estado) {
    struct Atraccion *atraccion;
    char *temp_estado;

    if (head_zonas == NULL) {
        return -1; /* ZONA NULA */
    }

    atraccion = buscar_atraccion_por_id(head_zonas, id_atraccion);
    if (atraccion == NULL) {
        return -2; /* ATRACCION NO ENCONTRADA */
    }

    temp_estado = copiar_string(nuevo_estado);
    if (temp_estado == NULL) {
        return -3; /* ERROR MEMORIA INSUFICIENTE */
    }

    free(atraccion->estado);
    atraccion->estado = temp_estado;

    if (strcmp(nuevo_estado, "cerrada") == 0 || strcmp(nuevo_estado, "fuera_de_servicio") == 0) {
        vaciar_filas_atraccion(head_zonas, id_atraccion);
    }

    return 0; /* EXITO */
}

/*====================================
---------------ENTRADAS---------------
======================================*/

int calcular_recaudacion_entradas(struct Parque *parque) {
    struct NodoEntradas *actual;
    int total_recaudado;

    if (parque == NULL || parque->head_entradas == NULL) {
        return 0;
    }

    total_recaudado = 0;
    actual = parque->head_entradas;

    while (actual != NULL) {
        if (actual->entrada != NULL) {
            if (actual->entrada->estado == NULL || strcmp(actual->entrada->estado, "anulada") != 0) {
                total_recaudado += actual->entrada->valor;
            }
        }

        actual = actual->sig;
    }

    return total_recaudado;
}

int comprar_entrada(struct NodoEntradas **entradas, char *tipo, int valor) {
    struct NodoEntradas *actual;
    struct Entrada *nueva_entrada;
    struct NodoEntradas *nuevo_nodo;
    int nuevo_id = 1;

    if (entradas == NULL || tipo == NULL) return -1;

    actual = *entradas;
    while (actual != NULL) {
        if (actual->entrada->id >= nuevo_id)
            nuevo_id = actual->entrada->id + 1;
        actual = actual->sig;
    }

    nueva_entrada = (struct Entrada *)malloc(sizeof(struct Entrada));
    nuevo_nodo = (struct NodoEntradas *)malloc(sizeof(struct NodoEntradas));

    if (nueva_entrada == NULL || nuevo_nodo == NULL) {
        if (nueva_entrada != NULL) free(nueva_entrada);
        if (nuevo_nodo != NULL) free(nuevo_nodo);
        return -1;
    }

    nueva_entrada->id = nuevo_id;
    nueva_entrada->valor = valor;

    nueva_entrada->tipo = copiar_string(tipo);
    if (nueva_entrada->tipo == NULL) {
        free(nueva_entrada);
        free(nuevo_nodo);
        return -1;
    }

    nueva_entrada->estado = copiar_string("activa");
    if (nueva_entrada->estado == NULL) {
        free(nueva_entrada->tipo);
        free(nueva_entrada);
        free(nuevo_nodo);
        return -1;
    }

    nuevo_nodo->entrada = nueva_entrada;
    nuevo_nodo->sig = *entradas;
    *entradas = nuevo_nodo;

    return nuevo_id;
}

int cambiar_estado_entrada(struct NodoEntradas **entradas, int id_entrada, char *nuevo_estado) {
    struct NodoEntradas *actual;
    char *copia_estado;

    if (entradas == NULL || nuevo_estado == NULL) return -1;

    actual = *entradas;
    while (actual != NULL) {
        if (actual->entrada->id == id_entrada) {
            copia_estado = copiar_string(nuevo_estado);
            if (copia_estado == NULL) return -1;
            free(actual->entrada->estado);
            actual->entrada->estado = copia_estado;
            return 1;
        }
        actual = actual->sig;
    }

    return -1;
}

int eliminar_entrada(struct NodoEntradas **entradas, int id_entrada) {
    struct NodoEntradas *actual;
    struct NodoEntradas *anterior;

    if (entradas == NULL) return -1;

    actual = *entradas;
    anterior = NULL;

    while (actual != NULL) {
        if (actual->entrada->id == id_entrada) {
            if (anterior == NULL)
                *entradas = actual->sig;
            else
                anterior->sig = actual->sig;

            free(actual->entrada->tipo);
            free(actual->entrada->estado);
            free(actual->entrada);
            free(actual);
            return 1;
        }
        anterior = actual;
        actual = actual->sig;
    }

    return -1;
}

struct Entrada *buscar_entrada_por_id(struct NodoEntradas *head_entradas, int id) {
    struct NodoEntradas *curr_nodo;

    if (head_entradas == NULL) {
        return NULL;
    }

    curr_nodo = head_entradas;
    while (curr_nodo != NULL) {

        if (curr_nodo->entrada != NULL && curr_nodo->entrada->id == id) {
            return curr_nodo->entrada;
        }

        curr_nodo = curr_nodo->sig;
    }
    return NULL;
}

/*====================================
-------------ESTADISTICAS-------------
======================================*/

void ver_reporte_general_dia(struct Parque *parque) {
    struct Atraccion *mayor_pico;
    struct Atraccion *mas_visitada;
    int total_dentro_parque, total_visitantes, mayor_espera, recaudacion;

    limpiar_pantalla();

    if (parque == NULL) {
        printf("[ERROR]: No se puede generar el reporte (Estructura del parque no inicializada).\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    mas_visitada = obtener_atraccion_mas_visitada(parque);
    mayor_pico = obtener_atraccion_mayor_pico(parque);

    if (mas_visitada == NULL || mayor_pico == NULL || mayor_pico->cap_max == 0) {
        printf("[ERROR]: No se puede generar el reporte (Datos insuficientes o inválidos en las atracciones).\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    mayor_espera = ((mayor_pico->pico_cola_general + mayor_pico->pico_cola_prioritaria) / mayor_pico->cap_max) * mayor_pico->duracion;

    recaudacion = calcular_recaudacion_entradas(parque);
    total_dentro_parque = total_personas_dentro_parque(parque);

    total_visitantes = total_personas_diario_parque(parque);

    printf("=========================================================================\n");
    printf("                      REPORTE GENERAL DIARIO DEL PARQUE\n");
    printf("=========================================================================\n\n");
    printf(" Flujo de Visitantes:\n");
    printf("  - Total de visitas registradas hoy  : %d\n", total_visitantes);
    printf("  - Personas activas dentro del parque: %d\n", total_dentro_parque);
    printf("-------------------------------------------------------------------------\n");
    printf(" Auditoría Financiera:\n");
    printf("  - Total recaudado por entradas      : $ %d\n", recaudacion);
    printf("-------------------------------------------------------------------------\n");
    printf(" Estadisticas de Atracciones:\n");
    printf("  - Atraccion mas concurrida          : %s (%d visitantes)\n",
           mas_visitada->nombre, mas_visitada->visitantes_totales);
    printf("  - Atraccion con mayor pico de cola  : %s\n",
           mayor_pico->nombre);
    printf("  - Tiempo maximo de espera calculado : %d min\n",
           mayor_espera);
    printf("=========================================================================\n\n");

    printf("Presione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_reporte_atracciones_actual(struct NodoZonas *head_zonas) {
    struct NodoZonas *zona_act;
    struct NodoAtraccion *atr_act;

    struct Atraccion *top_atracciones[10];
    struct Zona *top_zonas[10];

    int num_top;
    int i;
    int j;
    int pos;

    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("\n[ERROR] No hay zonas registradas.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    num_top = 0;
    for (i = 0; i < 10; i++) {
        top_atracciones[i] = NULL;
        top_zonas[i] = NULL;
    }

    zona_act = head_zonas;

    while (zona_act != NULL) {
        if (zona_act->datos != NULL) {
            atr_act = zona_act->datos->head_atracciones;

            while (atr_act != NULL) {
                if (atr_act->datos != NULL) {

                    pos = -1;
                    for (i = 0; i < 10; i++) {
                        if (top_atracciones[i] == NULL) {
                            pos = i;
                            break;
                        }
                        if (atr_act->datos->visitantes_totales > top_atracciones[i]->visitantes_totales) {
                            pos = i;
                            break;
                        }
                    }

                    if (pos != -1) {
                        for (j = 9; j > pos; j--) {
                            top_atracciones[j] = top_atracciones[j - 1];
                            top_zonas[j] = top_zonas[j - 1];
                        }
                        top_atracciones[pos] = atr_act->datos;
                        top_zonas[pos] = zona_act->datos;

                        if (num_top < 10) {
                            num_top++;
                        }
                    }
                }
                atr_act = atr_act->sig;
            }
        }
        zona_act = zona_act->sig;
    }

    if (num_top == 0) {
        printf("No hay atracciones registradas o con datos válidos en el parque.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    printf("\n=================================================================================\n");
    printf("                      RANKING TOP 10 ATRACCIONES MÁS VISITADAS                   \n");
    printf("=================================================================================\n");
    printf("%-4s | %-22s | %-15s | %-15s | %-10s\n", "Pos", "Atraccion", "Tematica", "Zona", "Visitas");
    printf("---------------------------------------------------------------------------------\n");

    for (i = 0; i < num_top; i++) {
        printf("%-4d | %-22s | %-15s | %-15s | %-10d\n",
               i + 1,
               top_atracciones[i]->nombre ? top_atracciones[i]->nombre : "Sin Nombre",
               top_atracciones[i]->tematica ? top_atracciones[i]->tematica : "Sin Tematica",
               top_zonas[i]->nombre ? top_zonas[i]->nombre : "Sin Zona",
               top_atracciones[i]->visitantes_totales);
    }
    printf("=================================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_atracciones_actuales(struct NodoZonas *head_zonas) {
    struct NodoZonas *zona_act;
    struct NodoAtraccion *atr_act;
    int contador_atracciones;

    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("Error: La lista de zonas del parque esta vacia.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    contador_atracciones = 0;
    zona_act = head_zonas;

    printf("\n====================================================================\n");
    printf("                    LISTADO GLOBAL DE ATRACCIONES                   \n");
    printf("====================================================================\n");
    printf("%-6s | %-25s | %s\n", "ID", "Nombre de la Atraccion", "Estado");
    printf("--------------------------------------------------------------------\n");

    while (zona_act != NULL) {
        if (zona_act->datos != NULL) {
            atr_act = zona_act->datos->head_atracciones;

            while (atr_act != NULL) {
                if (atr_act->datos != NULL) {

                    printf("%-6d | %-25s | %s\n",
                           atr_act->datos->id,
                           atr_act->datos->nombre ? atr_act->datos->nombre : "Sin Nombre",
                           atr_act->datos->estado ? atr_act->datos->estado : "Sin Estado");

                    contador_atracciones++;
                }
                atr_act = atr_act->sig;
            }
        }
        zona_act = zona_act->sig;
    }

    printf("--------------------------------------------------------------------\n");
    printf(" Total de atracciones actuales en el parque: %d\n", contador_atracciones);
    printf("====================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_zonas_actuales(struct NodoZonas *head_zonas) {
    struct NodoZonas *actual;
    int contador_zonas;
    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("[ERROR] La lista de zonas del parque esta vacía.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    contador_zonas = 0;
    actual = head_zonas;

    printf("\n================================================================================\n");
    printf("                            LISTADO GLOBAL DE ZONAS                             \n");
    printf("================================================================================\n");
    printf("%-6s | %-25s | %-20s | %s\n", "ID", "Nombre de la Zona", "Temática", "Visitantes Actuales");
    printf("--------------------------------------------------------------------------------\n");

    while (actual != NULL) {
        if (actual->datos != NULL) {

            printf("%-6d | %-25s | %-20s | %d\n",
                   actual->datos->id,
                   actual->datos->nombre ? actual->datos->nombre : "Sin Nombre",
                   actual->datos->tematica ? actual->datos->tematica : "Sin Temática",
                   actual->datos->visitantes_actuales);

            contador_zonas++;
        }
        actual = actual->sig;
    }

    printf("--------------------------------------------------------------------------------\n");
    printf(" Total de zonas actualmente registradas: %d\n", contador_zonas);
    printf("================================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

static void recorrer_e_imprimir_visitantes(struct NodoVisitantes *raiz, int *contador) {
    if (raiz == NULL) {
        return;
    }

    recorrer_e_imprimir_visitantes(raiz->izq, contador);

    if (raiz->datos != NULL) {
        printf("[%04d] %-18.18s ",
               raiz->datos->id,
               raiz->datos->nombre ? raiz->datos->nombre : "Sin Nombre");

        (*contador)++;

        if (*contador % 3 == 0) {
            printf("\n");
        } else {
            printf("| ");
        }
    }

    recorrer_e_imprimir_visitantes(raiz->der, contador);
}

void ver_visitantes_parque(struct NodoVisitantes *raiz_visitantes) {
    int total_visitantes;
    limpiar_pantalla();

    if (raiz_visitantes == NULL) {
        printf("[ERROR] El parque no tiene visitantes registrados en este momento.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    total_visitantes = 0;

    printf("\n===============================================================================\n");
    printf("                          REGISTRO GLOBAL DE VISITANTES                      \n");
    printf("===============================================================================\n");

    recorrer_e_imprimir_visitantes(raiz_visitantes, &total_visitantes);

    if (total_visitantes % 3 != 0) {
        printf("\n");
    }

    printf("-------------------------------------------------------------------------------\n");
    printf(" Total de visitantes actualmente en el parque: %d\n", total_visitantes);
    printf("===============================================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_entradas_compradas(struct NodoEntradas *head_entradas) {
    struct NodoEntradas *actual;
    int contador_entradas;
    limpiar_pantalla();

    if (head_entradas == NULL) {
        printf("[ERROR] No hay entradas registradas en el sistema.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    contador_entradas = 0;
    actual = head_entradas;

    printf("\n============================================================\n");
    printf("                REGISTRO GLOBAL DE ENTRADAS                 \n");
    printf("============================================================\n");
    printf("%-6s | %-15s | %-8s | %-12s\n", "ID", "Tipo", "Valor", "Estado");
    printf("------------------------------------------------------------\n");

    while (actual != NULL) {
        if (actual->entrada != NULL) {

            /* Se imprime la información alineada incluyendo el estado */
            printf("%-6d | %-15s | %-8d | %-12s\n",
                   actual->entrada->id,
                   actual->entrada->tipo ? actual->entrada->tipo : "Sin Tipo",
                   actual->entrada->valor,
                   actual->entrada->estado ? actual->entrada->estado : "Sin Estado");

            contador_entradas++;
        }
        actual = actual->sig;
    }

    printf("------------------------------------------------------------\n");
    printf(" Total de entradas emitidas: %d\n", contador_entradas);
    printf("============================================================\n\n");

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void ver_filas_atracciones(struct NodoZonas *head_zonas) {
    struct NodoZonas *zona_act;
    struct NodoAtraccion *atr_act;
    struct NodoFila *act_fila;
    int primero;
    int k;

    int total_p;
    int total_g;
    int eff_p;
    int eff_g;
    int ciclos_p;
    int ciclos_g;
    int t_espera_p;
    int t_espera_g;
    int cap_max;
    int duracion;

    limpiar_pantalla();

    if (head_zonas == NULL) {
        printf("[ERROR] No hay zonas registradas.\n");
        printf("\nPresione ENTER para regresar al menu principal...");
        while (getchar() != '\n');
        return;
    }

    zona_act = head_zonas;

    while (zona_act != NULL) {
        if (zona_act->datos != NULL) {
            atr_act = zona_act->datos->head_atracciones;

            while (atr_act != NULL) {
                if (atr_act->datos != NULL) {

                    cap_max = atr_act->datos->cap_max;
                    duracion = atr_act->datos->duracion;

                    total_p = 0;
                    act_fila = atr_act->datos->cola_prioritaria.frente;
                    while (act_fila != NULL) {
                        total_p += act_fila->tam_grupo;
                        act_fila = act_fila->sig;
                    }

                    total_g = 0;
                    act_fila = atr_act->datos->cola_general.frente;
                    while (act_fila != NULL) {
                        total_g += act_fila->tam_grupo;
                        act_fila = act_fila->sig;
                    }

                    eff_p = total_p + (total_p < total_g ? total_p : total_g);
                    eff_g = total_g + (total_g < total_p ? total_g : total_p);

                    ciclos_p = (eff_p + cap_max - 1) / cap_max;
                    ciclos_g = (eff_g + cap_max - 1) / cap_max;

                    t_espera_p = ciclos_p * duracion;
                    t_espera_g = ciclos_g * duracion;

                    printf("[%03d] %s (Ciclo: %d min | Cap Max: %d)\n",
                           atr_act->datos->id,
                           atr_act->datos->nombre ? atr_act->datos->nombre : "Sin Nombre",
                           duracion, cap_max);

                    printf("  Prioritaria : ");
                    act_fila = atr_act->datos->cola_prioritaria.frente;
                    primero = 1;
                    while (act_fila != NULL) {
                        for (k = 0; k < act_fila->tam_grupo; k++) {
                            if (!primero) printf(" -> ");
                            printf("%d", act_fila->ids_grupo[k]);
                            primero = 0;
                        }
                        act_fila = act_fila->sig;
                    }
                    if (primero) {
                        printf("(Vacia)");
                    } else {
                        printf(" | Espera: %d min (%d pers.)", t_espera_p, total_p);
                    }
                    printf("\n");

                    printf("  General     : ");
                    act_fila = atr_act->datos->cola_general.frente;
                    primero = 1;
                    while (act_fila != NULL) {
                        for (k = 0; k < act_fila->tam_grupo; k++) {
                            if (!primero) printf(" -> ");
                            printf("%d", act_fila->ids_grupo[k]);
                            primero = 0;
                        }
                        act_fila = act_fila->sig;
                    }
                    if (primero) {
                        printf("(Vacia)");
                    } else {
                        printf(" | Espera: %d min (%d pers.)", t_espera_g, total_g);
                    }

                    printf("\n---------------------------------------------------------\n");
                }
                atr_act = atr_act->sig;
            }
        }
        zona_act = zona_act->sig;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

/*====================================
----------------FILAS-----------------
======================================*/

void vaciar_cola_nodos(struct Fila *fila) {
    struct NodoFila *actual;
    struct NodoFila *siguiente;

    if (fila == NULL) {
        return;
    }

    actual = fila->frente;

    while (actual != NULL) {
        siguiente = actual->sig;
        free(actual);
        actual = siguiente;
    }

    fila->frente = NULL;
    fila->final = NULL;
}

int vaciar_filas_atraccion(struct NodoZonas *head_zonas, int id_atraccion) {
    struct Atraccion *atraccion;

    atraccion = buscar_atraccion_por_id(head_zonas, id_atraccion);
    if (atraccion == NULL) {
        return -1;
    }

    vaciar_cola_nodos(&(atraccion->cola_general));
    vaciar_cola_nodos(&(atraccion->cola_prioritaria));

    return 0;
}

int agregar_grupo_fila(struct NodoVisitantes *raiz_visitantes, struct Atraccion *atraccion, int *ids_grupo, int tam_grupo, int es_prioritaria) {
    int i;
    struct Visitante *v;
    struct NodoFila *nuevo_nodo;
    struct Fila *fila_destino;
    struct NodoFila *actual;
    int personas_en_cola;

    if (tam_grupo < 1 || tam_grupo > 10) {
        return -6;
    }

    if (atraccion == NULL || atraccion->estado == NULL || strcmp(atraccion->estado, "operativa") != 0) {
        return -1;
    }

    for (i = 0; i < tam_grupo; i++) {
        v = buscar_visitante_por_id(raiz_visitantes, ids_grupo[i]);

        if (v == NULL) {
            return -2;
        }

        if (v->edad < atraccion->edad_min || v->altura < atraccion->altura_min) {
            return -3;
        }

        if (es_prioritaria) {
            if (v->entrada == NULL || v->entrada->tipo == NULL || strcmp(v->entrada->tipo, "prioritaria") != 0) {
                return -4;
            }
        }
    }

    nuevo_nodo = (struct NodoFila *)malloc(sizeof(struct NodoFila));
    if (nuevo_nodo == NULL) {
        return -5;
    }

    nuevo_nodo->tam_grupo = tam_grupo;
    nuevo_nodo->sig = NULL;

    for (i = 0; i < 10; i++) {
        if (i < tam_grupo) {
            nuevo_nodo->ids_grupo[i] = ids_grupo[i];
        } else {
            nuevo_nodo->ids_grupo[i] = 0;
        }
    }

    if (es_prioritaria) {
        fila_destino = &(atraccion->cola_prioritaria);
    } else {
        fila_destino = &(atraccion->cola_general);
    }

    if (fila_destino->final == NULL) {
        fila_destino->frente = nuevo_nodo;
        fila_destino->final = nuevo_nodo;
    } else {
        fila_destino->final->sig = nuevo_nodo;
        fila_destino->final = nuevo_nodo;
    }

    atraccion->visitantes_totales += tam_grupo;

    personas_en_cola = 0;
    actual = fila_destino->frente;
    while (actual != NULL) {
        personas_en_cola += actual->tam_grupo;
        actual = actual->sig;
    }

    if (es_prioritaria) {
        if (personas_en_cola > atraccion->pico_cola_prioritaria) {
            atraccion->pico_cola_prioritaria = personas_en_cola;
        }
    } else {
        if (personas_en_cola > atraccion->pico_cola_general) {
            atraccion->pico_cola_general = personas_en_cola;
        }
    }

    return 0;
}

int avanzar_fila_atraccion(struct NodoZonas *head_zonas, int id_atraccion) {
    struct Atraccion *atraccion;
    int cap_disponible;
    int turno_prioritaria;
    int p_bloqueada;
    int g_bloqueada;
    int encontrado;

    struct Fila *fila_actual;
    int *bloqueada_actual;
    int *bloqueada_otra;
    struct NodoFila *temp;
    struct NodoFila *anterior;
    struct NodoFila *actual;

    atraccion = buscar_atraccion_por_id(head_zonas, id_atraccion);
    if (atraccion == NULL) {
        return -1;
    }

    if (atraccion->cola_prioritaria.frente == NULL && atraccion->cola_general.frente == NULL) {
        return -2;
    }

    cap_disponible = atraccion->cap_max;
    turno_prioritaria = 1;
    p_bloqueada = 0;
    g_bloqueada = 0;

    while (cap_disponible > 0 && (!p_bloqueada || !g_bloqueada)) {

        if (turno_prioritaria) {
            fila_actual = &(atraccion->cola_prioritaria);
            bloqueada_actual = &p_bloqueada;
            bloqueada_otra = &g_bloqueada;
        } else {
            fila_actual = &(atraccion->cola_general);
            bloqueada_actual = &g_bloqueada;
            bloqueada_otra = &p_bloqueada;
        }

        if (fila_actual->frente == NULL || *bloqueada_actual) {
            *bloqueada_actual = 1;
            if (*bloqueada_otra) {
                break;
            }
            turno_prioritaria = !turno_prioritaria;
            continue;
        }

        /* CASO A: El grupo en el frente de la fila cabe perfectamente */
        if (fila_actual->frente->tam_grupo <= cap_disponible) {
            temp = fila_actual->frente;
            cap_disponible -= temp->tam_grupo;

            fila_actual->frente = temp->sig;
            if (fila_actual->frente == NULL) {
                fila_actual->final = NULL;
            }
            free(temp);

            /* CAMBIO: Solo alternar si la otra cola está operativa */
            if (!*bloqueada_otra) {
                turno_prioritaria = !turno_prioritaria;
            }
        }

        /* CASO B: El grupo del frente excede la capacidad. Buscamos atrás. */
        else {
            anterior = fila_actual->frente;
            actual = fila_actual->frente->sig;
            encontrado = 0;

            while (actual != NULL) {
                if (actual->tam_grupo <= cap_disponible) {
                    encontrado = 1;
                    break;
                }
                anterior = actual;
                actual = actual->sig;
            }

            if (encontrado) {
                cap_disponible -= actual->tam_grupo;

                anterior->sig = actual->sig;
                if (actual == fila_actual->final) {
                    fila_actual->final = anterior;
                }
                free(actual);

                if (!*bloqueada_otra) {
                    turno_prioritaria = !turno_prioritaria;
                }
            } else {
                *bloqueada_actual = 1;
                turno_prioritaria = !turno_prioritaria;
            }
        }
    }

    return 0;
}

/*====================================
-------------VISITANTES---------------
======================================*/

struct Visitante* buscar_visitante_por_id(struct NodoVisitantes *raiz, int id) {
    if (raiz == NULL || raiz->datos == NULL) {
        return NULL;
    }

    if (raiz->datos->id == id) {
        return raiz->datos;
    }

    if (id < raiz->datos->id) {
        return buscar_visitante_por_id(raiz->izq, id);
    }

    return buscar_visitante_por_id(raiz->der, id);
}

int contar_dentro_parque(struct NodoVisitantes *raiz) {
    int cuenta;

    if (raiz == NULL) {
        return 0;
    }

    cuenta = 0;

    if (raiz->datos != NULL &&
        raiz->datos->entrada != NULL &&
        raiz->datos->entrada->estado != NULL) {

        if (strcmp(raiz->datos->entrada->estado, "utilizada") == 0) {
            cuenta = 1;
        }
    }

    return cuenta + contar_dentro_parque(raiz->izq) + contar_dentro_parque(raiz->der);
}

int contar_total_visitantes(struct NodoVisitantes *raiz) {
    int cuenta;

    if (raiz == NULL) {
        return 0;
    }

    cuenta = (raiz->datos != NULL) ? 1 : 0;

    return cuenta + contar_total_visitantes(raiz->izq) + contar_total_visitantes(raiz->der);
}

int total_personas_diario_parque(struct Parque *parque) {
    if (parque == NULL) {
        return 0;
    }
    return contar_total_visitantes(parque->raiz_visitantes);
}

int total_personas_dentro_parque(struct Parque *parque) {
    if (parque == NULL) {
        return 0;
    }
    return contar_dentro_parque(parque->raiz_visitantes);
}

static int id_existe_en_arbol(struct NodoVisitantes *raiz, int id) {
    struct NodoVisitantes *actual;
    actual = raiz;

    while (actual != NULL) {
        if (actual->datos != NULL) {
            if (id == actual->datos->id) {
                return 1; /* ID encontrada */
            }
            if (id < actual->datos->id) {
                actual = actual->izq;
            } else {
                actual = actual->der;
            }
        } else {
            break;
        }
    }
    return 0; /* ID libre */
}

int agregar_visitante(struct Parque *parque, struct Entrada *entrada, char *nombre, char *rut, int edad, float altura) {
    int nueva_id;
    struct NodoVisitantes **enlace;
    struct NodoVisitantes *nuevo_nodo;
    struct Visitante *nuevo_visitante;

    if (parque == NULL) {
        return -1; /* ERR PARQUE NULO */
    }

    if (strcmp(entrada->estado, "utilizada") == 0) {
        return -4; /* entrada ya en uso*/
    }

    if (contar_dentro_parque(parque->raiz_visitantes) + 1 > parque->cap_max){
        return -3; /* Capacidad máxima de parque alcanzada */
    }

    nueva_id = 1;
    while (id_existe_en_arbol(parque->raiz_visitantes, nueva_id)) {
        nueva_id++;
    }

    nuevo_nodo = (struct NodoVisitantes *)malloc(sizeof(struct NodoVisitantes));
    nuevo_visitante = (struct Visitante *)malloc(sizeof(struct Visitante));

    if (nuevo_nodo == NULL || nuevo_visitante == NULL) {
        free(nuevo_nodo);
        free(nuevo_visitante);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    nuevo_visitante->nombre = copiar_string(nombre);
    if (nuevo_visitante->nombre == NULL) {
        free(nuevo_visitante);
        free(nuevo_nodo);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    strncpy(nuevo_visitante->rut, rut, 10);
    nuevo_visitante->rut[10] = '\0';

    nuevo_visitante->id = nueva_id;
    nuevo_visitante->edad = edad;
    nuevo_visitante->altura = altura;
    nuevo_visitante->entrada = entrada;
    if (nuevo_visitante->entrada->estado != NULL) {
        free(nuevo_visitante->entrada->estado);
    }
    nuevo_visitante->entrada->estado = copiar_string("utilizada");

    nuevo_nodo->datos = nuevo_visitante;
    nuevo_nodo->izq = NULL;
    nuevo_nodo->der = NULL;

    enlace = &parque->raiz_visitantes;
    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL) {
            if (nueva_id < (*enlace)->datos->id) {
                enlace = &((*enlace)->izq);
            } else {
                enlace = &((*enlace)->der);
            }
        } else {
            break;
        }
    }

    *enlace = nuevo_nodo;

    return 0;
}

int eliminar_visitante(struct NodoVisitantes **raiz_visitantes, int id) {
    struct NodoVisitantes **enlace;
    struct NodoVisitantes *a_eliminar;
    struct NodoVisitantes **sucesor;
    struct NodoVisitantes *nodo_sucesor;

    if (raiz_visitantes == NULL || *raiz_visitantes == NULL) {
        return -1; /* ERR ARBOL VACIO O NULO */
    }

    enlace = raiz_visitantes;
    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL) {
            if (id == (*enlace)->datos->id) {
                break;
            }
            if (id < (*enlace)->datos->id) {
                enlace = &((*enlace)->izq);
            } else {
                enlace = &((*enlace)->der);
            }
        } else {
            return -3; /* ERR ESTRUCTURA CORRUPTA */
        }
    }

    if (*enlace == NULL) {
        return -2; /* ERR VISITANTE NO ENCONTRADO */
    }

    a_eliminar = *enlace;

    if (a_eliminar->izq == NULL) {
        *enlace = a_eliminar->der;

        if (a_eliminar->datos != NULL) {
            free(a_eliminar->datos->nombre);
            free(a_eliminar->datos);
        }
        free(a_eliminar);
    }
    else if (a_eliminar->der == NULL) {
        *enlace = a_eliminar->izq;

        if (a_eliminar->datos != NULL) {
            free(a_eliminar->datos->nombre);
            free(a_eliminar->datos);
        }
        free(a_eliminar);
    }
    else {
        sucesor = &(a_eliminar->der);
        while ((*sucesor)->izq != NULL) {
            sucesor = &((*sucesor)->izq);
        }

        nodo_sucesor = *sucesor;

        if (a_eliminar->datos != NULL) {
            free(a_eliminar->datos->nombre);
            free(a_eliminar->datos);
        }

        a_eliminar->datos = nodo_sucesor->datos;

        *sucesor = nodo_sucesor->der;

        free(nodo_sucesor);
    }

    return 0;
}

/*====================================
-----------------ZONAS----------------
======================================*/

struct Zona *obtener_zona_por_id(struct NodoZonas *head_zonas, int id_zona) {
    struct NodoZonas *actual;

    if (head_zonas == NULL) {
        return NULL;
    }

    actual = head_zonas;

    while (actual != NULL) {
        if (actual->datos != NULL && actual->datos->id == id_zona)
            return actual->datos;
        actual = actual->sig;
    }

    return NULL;
}

int agregar_zona(struct NodoZonas **head_zonas, const char *nombre, const char *tematica,
                 int cap_max, int hora_apertura, int hora_cierre, int min_apertura,
                 int min_cierre, int max_atracciones) {

    int nueva_id;
    int id_ocupada;
    struct NodoZonas *actual;
    struct NodoZonas *nuevo_nodo;
    struct Zona *nueva_zona;

    if (head_zonas == NULL) {
        return -1; /* ERR PUNTERO RAIZ NULO */
    }

    nueva_id = 1;
    id_ocupada = 1;
    while (id_ocupada) {
        id_ocupada = 0;
        actual = *head_zonas;
        while (actual != NULL) {
            if (actual->datos != NULL && actual->datos->id == nueva_id) {
                id_ocupada = 1;
                break;
            }
            actual = actual->sig;
        }
        if (id_ocupada) {
            nueva_id++;
        }
    }

    nuevo_nodo = (struct NodoZonas *)malloc(sizeof(struct NodoZonas));
    nueva_zona = (struct Zona *)malloc(sizeof(struct Zona));

    if (nuevo_nodo == NULL || nueva_zona == NULL) {
        free(nuevo_nodo);
        free(nueva_zona);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    nueva_zona->nombre = copiar_string(nombre);
    nueva_zona->tematica = copiar_string(tematica);

    if (nueva_zona->nombre == NULL || nueva_zona->tematica == NULL) {
        free(nueva_zona->nombre);
        free(nueva_zona->tematica);
        free(nueva_zona);
        free(nuevo_nodo);
        return -2; /* ERR MEMORIA INSUFICIENTE */
    }

    nueva_zona->id = nueva_id;
    nueva_zona->cap_max = cap_max;
    nueva_zona->visitantes_actuales = 0;
    nueva_zona->atracciones_max = max_atracciones;
    nueva_zona->head_atracciones = NULL;

    nueva_zona->hora_apertura.hora = hora_apertura;
    nueva_zona->hora_apertura.minutos = min_apertura;
    nueva_zona->hora_cierre.hora = hora_cierre;
    nueva_zona->hora_cierre.minutos = min_cierre;

    nuevo_nodo->datos = nueva_zona;
    nuevo_nodo->sig = *head_zonas;
    *head_zonas = nuevo_nodo;

    return 0;
}

int eliminar_zona(struct NodoZonas **head_zonas, int id) {
    struct NodoZonas **enlace;
    struct NodoZonas *a_eliminar;
    struct NodoAtraccion *act_atr;
    struct NodoAtraccion *sig_atr;
    struct NodoFila *act_fila;
    struct NodoFila *sig_fila;

    if (head_zonas == NULL || *head_zonas == NULL) {
        return -1; /* LISTA VACIA */
    }

    enlace = head_zonas;
    while (*enlace != NULL) {
        if ((*enlace)->datos != NULL && (*enlace)->datos->id == id) {
            break;
        }
        enlace = &((*enlace)->sig);
    }

    if (*enlace == NULL) {
        return -2; /* ZONA NO ENCONTRADA */
    }

    a_eliminar = *enlace;
    *enlace = a_eliminar->sig;

    if (a_eliminar->datos != NULL) {

        act_atr = a_eliminar->datos->head_atracciones;
        while (act_atr != NULL) {
            sig_atr = act_atr->sig;
            if (act_atr->datos != NULL) {

                act_fila = act_atr->datos->cola_general.frente;
                while (act_fila != NULL) {
                    sig_fila = act_fila->sig;
                    free(act_fila);
                    act_fila = sig_fila;
                }

                act_fila = act_atr->datos->cola_prioritaria.frente;
                while (act_fila != NULL) {
                    sig_fila = act_fila->sig;
                    free(act_fila);
                    act_fila = sig_fila;
                }

                free(act_atr->datos->nombre);
                free(act_atr->datos->tematica);
                free(act_atr->datos->estado);

                free(act_atr->datos);
            }

            free(act_atr);
            act_atr = sig_atr;
        }

        free(a_eliminar->datos->nombre);
        free(a_eliminar->datos->tematica);
        free(a_eliminar->datos);
    }

    free(a_eliminar);

    return 0;
}

int agregar_o_remover_visitantes_zona(struct NodoZonas *head_zonas, int id, int visitantes) {
    struct NodoZonas *actual;
    int nuevo_total;

    struct Tiempo hora_actual;
    int mins_actuales;
    int mins_apertura;
    int mins_cierre;

    if (head_zonas == NULL) {
        return -1; /* LISTA NULA */
    }

    actual = head_zonas;

    while (actual != NULL) {
        if (actual->datos != NULL && actual->datos->id == id) {

            if (visitantes > 0) {
                hora_actual = obtener_hora_actual();

                mins_actuales = (hora_actual.hora * 60) + hora_actual.minutos;
                mins_apertura = (actual->datos->hora_apertura.hora * 60) + actual->datos->hora_apertura.minutos;
                mins_cierre = (actual->datos->hora_cierre.hora * 60) + actual->datos->hora_cierre.minutos;

                if (mins_actuales < mins_apertura || mins_actuales >= mins_cierre) {
                    return -5; /* ZONA CERRADA POR HORARIO */
                }
            }

            nuevo_total = actual->datos->visitantes_actuales + visitantes;

            if (nuevo_total > actual->datos->cap_max) {
                return -3; /* SUPERA CANTIDAD MAXIMA */
            }

            if (nuevo_total < 0) {
                return -4; /* VISITANTES MENOR A 0 */
            }

            actual->datos->visitantes_actuales = nuevo_total;
            return 0; /* ÉXITO */
        }
        actual = actual->sig;
    }

    return -2; /* ZONA NO ENCONTRADA */
}

/*
====================================
---------------MENUS----------------
====================================
*/

int escoger_opcion(void) {
    char linea[100], basura;
    int asignados, numero;

    while (1) {
        printf(">> ");
        if (fgets(linea, sizeof(linea), stdin) != NULL) {
            asignados = sscanf(linea, "%d %c", &numero, &basura);
            if (asignados == 1)
                return numero;
        }

        printf("[ERROR] Entrada no permitida. Intente de nuevo.\n\n");
    }
}


int menu_inicializar_parque(struct Parque *parque) {
    char linea[256];
    char token[30];
    char basura;
    int cap_max_buf;
    int general_buf;
    int infantil_buf;
    int familiar_buf;
    int prioritaria_buf;
    int asignados;
    int es_valido;

    es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                       ++ BIENVENIDO A IBCLANDIA ++\n"
        "=========================================================================\n"
        "     Para empezar a manejar el sistema el día de hoy, debe ingresar\n"
        "          los siguientes parámetros en una sola línea y en orden.\n"
        "                  Escriba '0' para cerrar el programa\n"
        "=========================================================================\n\n"
    );
    printf(
        " - Los parámetros son (separados por espacios):\n"
        "[1] Capacidad Máxima de Visitantes del Parque \n"
        "[2] Precio entrada GENERAL\n"
        "[3] Precio entrada INFANTIL\n"
        "[4] Precio entrada FAMILIAR\n"
        "[5] Precio entrada PRIORITARIA\n"
        "Por ejemplo: '200000 5000 2500 10000 8000'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "0") == 0) {
                return 0;
            }
        }

        asignados = sscanf(linea, "%d %d %d %d %d %c",
                           &cap_max_buf, &general_buf, &infantil_buf,
                           &familiar_buf, &prioritaria_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 5) {
            printf("[ERROR] Datos incompletos. Se procesaron solo %d de los 5 campos requeridos.\n\n", asignados);
            continue;
        }

        if (asignados > 5) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (cap_max_buf <= 0 || general_buf < 0 || infantil_buf < 0 || familiar_buf < 0 || prioritaria_buf < 0) {
            printf("[ERROR] Valores inválidos. La capacidad debe ser positiva y los precios no pueden ser negativos.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (parque != NULL) {
        parque->cap_max = cap_max_buf;

        parque->raiz_visitantes = NULL;
        parque->head_zonas = NULL;
        parque->head_entradas = NULL;

        ENTRADA_GENERAL = general_buf;
        ENTRADA_INFANTIL = infantil_buf;
        ENTRADA_FAMILIAR = familiar_buf;
        ENTRADA_PRIORITARIA = prioritaria_buf;

        printf("[SISTEMA] ¡Parque inicializado con éxito!\n");
    } else {
        printf("[ERROR] El puntero al parque provisto es nulo (NULL). No se pudo inicializar.\n");
    }

    printf("\nPresione ENTER para continuar...");
    while (getchar() != '\n');
    return 1;
}

void mostrar_menu_principal(void) {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                         ++ SISTEMA IBCLANDIA ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n"
        "                          Para salir escriba '0'\n"
        "=========================================================================\n\n");
    printf(
        "- CONTROL Y MANEJO DE PERSONAS\n"
        "[1] Control y Compra de Entradas\n"
        "[2] Manejo de Visitantes en el Parque\n"
        "[3] Control Filas de Atracciones\n\n"

        "- GESTIÓN DEL PARQUE\n"
        "[4] Gestionar Zonas\n"
        "[5] Gestionar Atracciones\n\n"
    );
    printf(
        "- REPORTES Y ESTADÍSTICAS\n"
        "[6] Ver Reporte General Del Día\n"
        "[7] Ver Reporte Sobre Atracciones Actual\n\n"

        "- INFORMACIÓN DEL PARQUE\n"
        "[8] Ver Atracciones Actuales\n"
        "[9] Ver Zonas Actuales\n"
        "[10] Ver Visitantes en el Parque\n"
        "[11] Ver Entradas Compradas\n"
        "[12] Ver Filas de Atracción\n\n"

        "=========================================================================\n\n"
    );
}


void mostrar_submenu_entradas() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                              ++ ENTRADAS ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n"
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n");
    printf(
        "[1] Comprar Entrada\n"
        "[2] Eliminar Entrada del Sistema\n"
        "[3] Cambiar Estado de Entrada\n\n"

        "=========================================================================\n\n"
    );
}

void menu_comprar_entrada(struct NodoEntradas **entradas) {
    char linea[150];
    char tipo_buf[30];
    int cantidad_buf;
    char basura;
    int asignados;
    int es_valido;

    int i;
    int precio_base;
    int precio_ticket;
    int exitos;

    precio_base = 0;
    exitos = 0;
    es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                          ++ COMPRAR ENTRADA ++\n"
        "=========================================================================\n"
        " Para comprar una entrada y agregarla, rellene los siguientes parámetros \n"
        "                        in el orden que se indica.\n"
        "               Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Tipo de Entrada y Cantidad a Comprar\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] Tipos de entrada: 'general', 'infantil', 'familiar', y 'prioritaria'\n"
        "[4] Comprar la entrada familiar permite el ingreso gratuito de hasta otros\n"
        "    tres miembros por el precio de 1 entrada. Por lo que la cantidad máxima\n"
        "    por esta entrada es 4.\n"
        "Por ejemplo: 'prioritaria 4'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer la entrada por teclado.\n\n");
            continue;
        }

        asignados = sscanf(linea, "%29s %d %c", tipo_buf, &cantidad_buf, &basura);

        if (asignados >= 1 && strcmp(tipo_buf, "volver") == 0) {
            return;
        }

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ninguna entrada válida. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el segundo parámetro. Debe ingresar la cantidad después del tipo.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió demasiados parámetros (máximo 2).\n\n");
            continue;
        }

        if (strcmp(tipo_buf, "general") != 0 &&
            strcmp(tipo_buf, "infantil") != 0 &&
            strcmp(tipo_buf, "familiar") != 0 &&
            strcmp(tipo_buf, "prioritaria") != 0) {

            printf("[ERROR] El tipo '%s' no es valido.\n", tipo_buf);
            printf("[INFO] Tipos permitidos estrictamente en minúsculas: [general, infantil, familiar, prioritaria]\n\n");
            continue;
        }

        if (cantidad_buf <= 0) {
            printf("[ERROR] La cantidad (%d) no es válida. Debe ser mayor a cero.\n\n", cantidad_buf);
            continue;
        }

        if (strcmp(tipo_buf, "familiar") == 0 && cantidad_buf > 4) {
            printf("[ERROR] La cantidad máxima permitida para entradas de tipo 'familiar' es de 4.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (strcmp(tipo_buf, "general") == 0) {
        precio_base = ENTRADA_GENERAL;
    } else if (strcmp(tipo_buf, "infantil") == 0) {
        precio_base = ENTRADA_INFANTIL;
    } else if (strcmp(tipo_buf, "familiar") == 0) {
        precio_base = ENTRADA_FAMILIAR;
    } else if (strcmp(tipo_buf, "prioritaria") == 0) {
        precio_base = ENTRADA_PRIORITARIA;
    }

    for (i = 0; i < cantidad_buf; i++) {
        if (strcmp(tipo_buf, "familiar") == 0 && i > 0) {
            precio_ticket = 0;
        } else {
            precio_ticket = precio_base;
        }

        if (comprar_entrada(entradas, tipo_buf, precio_ticket)) {
            exitos++;
        }
    }

    if (exitos == cantidad_buf) {
        printf("[SISTEMA] ¡%d entrada(s) de tipo '%s' registrada(s) y comprada(s) con éxito!\n", exitos, tipo_buf);
    } else if (exitos > 0) {
        printf("[ALERTA] Operación parcial: Solo se registraron %d de %d entradas solicitadas.\n", exitos, cantidad_buf);
    } else {
        printf("[ALERTA] El sistema rechazó la operacion de compra.\n");
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void menu_eliminar_entrada(struct NodoEntradas **entradas) {
    char linea[100];
    char token[30];
    int id_buf = 0;
    char basura;
    int asignados;
    int es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                           ++ ELIMINAR ENTRADA ++\n"
        "=========================================================================\n"
        " Para eliminar una entrada del parque, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "               Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(

        "- INFORMACIÓN\n"
        "[1] El parámetro es únicamente la ID de la entrada\n"
        "Por ejemplo: '345'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %c", &id_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numerico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada invalida. Escribio argumentos de mas.\n\n");
            continue;
        }

        if (id_buf <= 0) {
            printf("[ERROR] ID invalido. Los identificadores deben ser mayores a cero.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (eliminar_entrada(entradas, id_buf)) {
        printf("[SISTEMA] Entrada con ID %d eliminada exitosamente.\n", id_buf);
    } else {
        printf("[ERROR] No se pudo eliminar: No se encontro ninguna entrada con el ID %d.\n", id_buf);
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
    return;
}

void menu_cambiar_estado_entrada(struct NodoEntradas **entradas) {
    char linea[150];
    char token_control[30];
    char estado_buf[30];
    int id_buf;
    char basura;
    int asignados;
    int es_valido;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                     ++ CAMBIAR ESTADO DE ENTRADA ++\n"
        "=========================================================================\n"
        " Para cambiar el estado de la entrada, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID de la entrada, y el nuevo estado\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] Los estados de entrada son: 'activa', 'utilizada', 'vencida', y 'anulada'\n"
        "Por ejemplo: '25 anulada'\n\n"

        "=========================================================================\n\n"
    );

    es_valido = 0;

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error critico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %29s %c", &id_buf, estado_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. El primer parámetro debe ser el ID numérico de la entrada.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el segundo parámetro. Debe ingresar el nuevo estado despues del ID.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió parámetros de más (máximo 2).\n\n");
            continue;
        }

        if (id_buf <= 0) {
            printf("[ERROR] El ID de la entrada (%d) no es valido. Debe ser mayor a cero.\n\n", id_buf);
            continue;
        }

        if (strcmp(estado_buf, "activa") != 0 &&
            strcmp(estado_buf, "utilizada") != 0 &&
            strcmp(estado_buf, "vencida") != 0 &&
            strcmp(estado_buf, "anulada") != 0) {

            printf("[ERROR] El estado '%s' no es valido.\n", estado_buf);
            printf("[INFO] Estados permitidos: [activa, utilizada, vencida, anulada]\n\n");
            continue;
        }


        es_valido = 1;
    }

    if (cambiar_estado_entrada(entradas, id_buf, estado_buf)) {
        printf("[SISTEMA] Estado de la entrada %d actualizado a '%s' con éxito\n", id_buf, estado_buf);
    } else {
        printf("[ERROR] No se encontró ninguna entrada con el ID %d o no se pudo modificar.\n", id_buf);
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}


void mostrar_submenu_visitantes() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                             ++ VISITANTES ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n"
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n");
    printf(
        "[1] Agregar Visitante al Parque\n"
        "[2] Eliminar Visitante del Parque\n\n"

        "=========================================================================\n\n"

    );
}

void menu_agregar_visitante(struct Parque *parque) {
    char linea[256];
    char token_control[40];
    char nombre_buf[40];
    char rut_buf[15];
    char basura;

    int edad_buf;
    float altura_buf;
    int id_entrada_buf;

    int asignados;
    int es_valido;
    int resultado_logico;
    struct Entrada *entrada_visitante;

    es_valido = 0;
    entrada_visitante = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                          ++ AGREGAR VISITANTE ++\n"
        "=========================================================================\n"
        "      Para agregar a un visitante, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Nombre, RUT, Edad, Altura (mts) y la ID de Entrada\n"
        "[2] Deben escribirse en conjunto y separados por un espacio\n"
        "[3] El nombre no debe contener espacios (ver ejemplo)\n"
        "Por ejemplo: 'Juan_Perez 15936475-1 45 1.72 34'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%39s %14s %d %f %d %c",
                           nombre_buf, rut_buf, &edad_buf, &altura_buf, &id_entrada_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 5) {
            printf("[ERROR] Datos incompletos. Se procesaron solo %d de los 5 campos requeridos.\n\n", asignados);
            continue;
        }

        if (asignados > 5) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (edad_buf < 0 || edad_buf > 120 || altura_buf <= 0.40f || altura_buf > 2.50f || id_entrada_buf <= 0) {
            printf("[ERROR] Valores numéricos incoherentes. Verifique edad, altura e ID de entrada.\n\n");
            continue;
        }

        entrada_visitante = buscar_entrada_por_id(parque->head_entradas, id_entrada_buf);
        if (entrada_visitante == NULL) {
            printf("[ERROR] La entrada con ID %d no existe o no está registrada en el sistema.\n\n", id_entrada_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = agregar_visitante(parque, entrada_visitante, nombre_buf, rut_buf, edad_buf, altura_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] ¡Visitante '%s' registrado exitosamente con la entrada ID %d!\n", nombre_buf, id_entrada_buf);
            break;

        case -1:
            printf("[ERROR] Error del sistema: Estructura del Parque no inicializada (NULL).\n");
            break;

        case -2:
            printf("[ERROR] Error de memoria: No se pudo reservar espacio para el nuevo visitante.\n");
            break;

        case -3:
            printf("[ERROR] El Parque llegó a su límite máximo.\n");
            break;
        case -4:
            printf("[ERROR] La entrada ya está en uso.");
            break;
        default:
            printf("[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void menu_eliminar_visitante(struct NodoVisitantes **raiz_visitantes) {
    char linea[100];
    char token[30];
    char basura;
    int id_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                        ++ ELIMINAR VISITANTE ++\n"
        "=========================================================================\n"
        "      Para eliminar a un visitante, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] El parámetro es únicamente la ID del visitante\n"
        "[2] La eliminación es permanente\n"
        "Por ejemplo: '67'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %c", &id_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numérico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = eliminar_visitante(raiz_visitantes, id_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] Visitante con ID %d eliminado exitosamente de los registros.\n", id_buf);
            break;

        case -1:
            printf("[ERROR] El árbol de visitantes está vacío o no ha sido inicializado (NULL).\n");
            break;

        case -2:
            printf("[ERROR] No se pudo eliminar: El visitante con ID %d no se encuentra registrado.\n", id_buf);
            break;

        case -3:
            printf("[ERROR] Estructura corrupta: Se detectó una anomalía crítica en la integridad del árbol.\n");
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}


void mostrar_submenu_filas() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                         ++ FILAS DE ATRACCIÓN ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n"
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n");
    printf(
        "[1] Agregar Grupo a la Fila\n"
        "[2] Avanzar Fila de Atracción\n\n"

        "=========================================================================\n\n"
    );
}

void menu_agregar_grupo_fila(struct Parque *parque) {
    char linea[256];
    char token_control[40];
    char tipo_buf[30];
    char basura;
    int ids_grupo[10];

    char *ptr_linea;
    int caracteres_leidos;
    int asignados;
    int tam_grupo;
    int id_atraccion;
    int es_prioritaria;
    int es_valido;
    int id_extra;
    int resultado_logico;
    struct Atraccion *atr_encontrada;

    es_valido = 0;
    tam_grupo = 0;
    id_atraccion = 0;
    atr_encontrada = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                        ++ AGREGAR GRUPO A FILA ++\n"
        "=========================================================================\n"
        " Para agregar un grupo a la atracción, rellene los siguientes parámetros\n"
        "                         en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: IDs del Grupo, Tipo de Fila, ID de Atracción\n"
        "[2] Máximo 10 IDs de integrantes pueden estar en el grupo.\n"
        "[3] Los tipo de fila pueden ser 'general' o 'prioritaria'\n"
        "[4] Todos los parámetros deben estar separados por un espacio\n"
        "Por ejemplo: '1 30 9 12 general 14'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        ptr_linea = linea;
        tam_grupo = 0;

        while (tam_grupo < 10 && sscanf(ptr_linea, "%d%n", &ids_grupo[tam_grupo], &caracteres_leidos) == 1) {
            tam_grupo++;
            ptr_linea += caracteres_leidos;
        }

        if (tam_grupo == 0) {
            printf("[ERROR] No se detectó ninguna ID de visitante al inicio. Intente de nuevo.\n\n");
            continue;
        }

        if (tam_grupo == 10) {
            if (sscanf(ptr_linea, "%d", &id_extra) == 1) {
                printf("[ERROR] Excedió el límite. El grupo puede contener un máximo de 10 visitantes.\n\n");
                continue;
            }
        }

        asignados = sscanf(ptr_linea, "%29s %d %c", tipo_buf, &id_atraccion, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Falta especificar el tipo de fila (general/prioritaria) y el ID de la atracción.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Falta el último parámetro: Ingrese el ID de la atracción de destino.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Detectados elementos desconocidos al final de la línea.\n\n");
            continue;
        }

        if (strcmp(tipo_buf, "general") != 0 && strcmp(tipo_buf, "prioritaria") != 0) {
            printf("[ERROR] Tipo de fila '%s' no válido.\n", tipo_buf);
            printf("[INFO] Debe escribir exactamente 'general' o 'prioritaria' en minúsculas.\n\n");
            continue;
        }

        if (id_atraccion <= 0) {
            printf("[ERROR] El ID de la atracción debe ser un número entero positivo.\n\n");
            continue;
        }

        if (parque != NULL) {
            atr_encontrada = buscar_atraccion_por_id(parque->head_zonas, id_atraccion);
        }

        if (atr_encontrada == NULL) {
            printf("[ERROR] La atracción con ID %d no existe en el sistema del parque.\n\n", id_atraccion);
            continue;
        }

        es_valido = 1;
    }

    es_prioritaria = (strcmp(tipo_buf, "prioritaria") == 0) ? 1 : 0;

    resultado_logico = agregar_grupo_fila(parque->raiz_visitantes, atr_encontrada, ids_grupo, tam_grupo, es_prioritaria);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] Grupo de %d visitantes enviado exitosamente a la fila '%s'\n", tam_grupo, tipo_buf);
            break;

        case -1:
            printf("[ERROR] Operación rechazada: La atracción seleccionada no se encuentra operativa actualmente.\n");
            break;

        case -2:
            printf("[ERROR] Validación fallida: Una o más IDs ingresadas no corresponden a visitantes registrados.\n");
            break;

        case -3:
            printf("[ERROR] Restricción física: Uno o más integrantes del grupo no cumplen con la edad o altura mínima.\n");
            break;

        case -4:
            printf("[ERROR] Acceso denegado: Algún integrante no posee una entrada prioritaria cuando el canal lo requiere.\n");
            break;

        case -5:
            printf("[ERROR] Error crítico: No hay suficiente espacio en la memoria dinámica del sistema.\n");
            break;

        case -10:
            printf("[ERROR] Tamaño inválido: La cantidad de integrantes proporcionada no es permitida para este grupo.\n");
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}

void menu_avanzar_fila_atraccion(struct NodoZonas *head_zonas) {
    char linea[100];
    char token[30];
    char basura;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;
    struct Atraccion *atr_encontrada;

    es_valido = 0;
    id_atraccion_buf = 0;
    atr_encontrada = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                     ++ AVANZAR FILAS DE ATRACCION ++\n"
        "=========================================================================\n"
        "           Para avanzar las filas, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] El único parámetro es la ID de la atracción\n"
        "[2] Esto avanzará ambas filas dependiendo de su capacidad\n"
        "Por ejemplo:'23'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %c", &id_atraccion_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numérico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_atraccion_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        atr_encontrada = buscar_atraccion_por_id(head_zonas, id_atraccion_buf);
        if (atr_encontrada == NULL) {
            printf("[ERROR] No encontrada: No se encontró ninguna atracción con el ID %d.\n\n", id_atraccion_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = avanzar_fila_atraccion(head_zonas, id_atraccion_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] ¡Filas de la atracción %d avanzadas con éxito!\n", id_atraccion_buf);
            break;

        case -1:
            printf("[ERROR] No encontrada: La atracción con ID %d no pudo ser localizada en el sistema de filas.\n", id_atraccion_buf);
            break;

        case -2:
            printf("[ERROR] Operación omitida: Ambas filas (general y prioritaria) de la atracción %d están vacías.\n", id_atraccion_buf);
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}


void mostrar_submenu_zonas() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                                ++ ZONAS ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n"
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n");
    printf(
        "[1] Agregar Zona al Parque\n"
        "[2] Eliminar Zona Del Parque\n"
        "[3] Agregar o Remover Visitantes de Zona\n\n"

        "=========================================================================\n\n"

    );

}

void menu_agregar_zona_al_parque(struct NodoZonas **head_zonas) {
    char linea[256];
    char token_control[40];
    char nombre_buf[40];
    char tematica_buf[30];
    char basura;

    int cap_max_buf;
    int h_ap_buf;
    int m_ap_buf;
    int h_ci_buf;
    int m_ci_buf;
    int max_atr_buf;

    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                            ++ AGREGAR ZONA ++\n"
        "=========================================================================\n"
        "         Para agregar una zona, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Nombre, Temática, Capacidad, Hora Apertura,\n"
        "Hora Cierre, y Máximo de Atracciones\n"
        "[2] Deben estar separados por un espacio y los nombres juntos.\n"
        "[3] Los tiempos están en formato de 24 hrs.\n"
        "Por ejemplo: 'Tren_de_la_Muerte Terror 1000 1:00 5:30 10'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%39s %29s %d %d:%d %d:%d %d %c",
                           nombre_buf, tematica_buf, &cap_max_buf,
                           &h_ap_buf, &m_ap_buf, &h_ci_buf, &m_ci_buf,
                           &max_atr_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 8) {
            printf("[ERROR] Datos incompletos o formato de hora incorrecto. Recuerde usar el formato HH:MM.\n");
            printf("[INFO] Se interpretaron con éxito sólo %d de los sub-parámetros.\n\n", asignados);
            continue;
        }

        if (asignados > 8) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (cap_max_buf <= 0 || max_atr_buf <= 0) {
            printf("[ERROR] La capacidad máxima y el máximo de atracciones deben ser números positivos.\n\n");
            continue;
        }

        if (h_ap_buf < 0 || h_ap_buf > 23 || m_ap_buf < 0 || m_ap_buf > 59) {
            printf("[ERROR] Hora de apertura inválida (%d:%02d). Ingrese un tiempo válido entre 00:00 y 23:59.\n\n", h_ap_buf, m_ap_buf);
            continue;
        }

        if (h_ci_buf < 0 || h_ci_buf > 23 || m_ci_buf < 0 || m_ci_buf > 59) {
            printf("[ERROR] Hora de cierre inválida (%d:%02d). Ingrese un tiempo válido entre 00:00 y 23:59.\n\n", h_ci_buf, m_ci_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = agregar_zona(head_zonas, nombre_buf, tematica_buf, cap_max_buf,
                                    h_ap_buf, h_ci_buf, m_ap_buf, m_ci_buf, max_atr_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] ¡Zona '%s' de temática '%s' agregada con éxito al parque!\n", nombre_buf, tematica_buf);
            break;

        case -1:
            printf("[ERROR] Raíz nula: La estructura de control (head_zonas) no está inicializada.\n");
            break;

        case -2:
            printf("[ERROR] Error de memoria: No hay suficiente espacio dinámico en el sistema.\n");
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}

void menu_eliminar_zona_del_parque(struct NodoZonas **head_zonas) {
    char linea[100];
    char token[30];
    char basura;
    int id_zona_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_zona_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                             ++ ELIMINAR ZONA ++\n"
        "=========================================================================\n"
        "       Para eliminar una zona, rellene los siguientes parámetros \n"
        "                        en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] El único parámetro es la ID de la Zona\n"
        "[2] Esto eliminará todas las atracciones dentro de la Zona\n"
        "Por ejemplo: '21'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %c", &id_zona_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numérico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_zona_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = eliminar_zona(head_zonas, id_zona_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] Zona con ID %d y todas sus atracciones asociadas eliminadas con éxito.\n", id_zona_buf);
            break;

        case -1:
            printf("[ERROR] Lista vacía: No existen zonas registradas en el parque actualmente.\n");
            break;

        case -2:
            printf("[ERROR] No encontrada: No se encontró ninguna zona con el ID %d en el sistema.\n", id_zona_buf);
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}

void menu_agregar_o_remover_visitante_zona(struct NodoZonas *head_zonas) {
    char linea[150];
    char token_control[40];
    char accion_buf[30];
    char basura;
    int cantidad_buf;
    int id_zona_buf;
    int asignados;
    int es_valido;
    int visitantes_param;
    int resultado_logico;

    es_valido = 0;
    cantidad_buf = 0;
    id_zona_buf = 0;
    visitantes_param = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                 ++ AGREGAR O REMOVER VISITANTE DE ZONA ++\n"
        "=========================================================================\n"
        " Para agregar o remover a un visitante, rellene los siguientes parámetros\n"
        "                        en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: Acción, Cantidad, ID de la Zona.\n"
        "[2] Debe usar 'agregar' o 'remover' como acciones.\n"
        "[3] Por ejemplo: 'remover 23 2'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer la entrada por teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%29s %d %d %c", accion_buf, &cantidad_buf, &id_zona_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 3) {
            printf("[ERROR] Datos incompletos. Debe ingresar la acción (agregar/remover), la cantidad y la ID de la zona.\n\n");
            continue;
        }

        if (asignados > 3) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (strcmp(accion_buf, "agregar") != 0 && strcmp(accion_buf, "remover") != 0) {
            printf("[ERROR] Acción '%s' no válida.\n", accion_buf);
            printf("[INFO] Use estrictamente las palabras 'agregar' o 'remover' en minúsculas.\n\n");
            continue;
        }

        if (cantidad_buf <= 0) {
            printf("[ERROR] La cantidad de visitantes debe ser un número entero positivo.\n\n");
            continue;
        }

        if (id_zona_buf <= 0) {
            printf("[ERROR] ID de la zona inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    if (strcmp(accion_buf, "agregar") == 0) {
        visitantes_param = cantidad_buf;
    } else {
        visitantes_param = -cantidad_buf;
    }

    resultado_logico = agregar_o_remover_visitantes_zona(head_zonas, id_zona_buf, visitantes_param);

    switch (resultado_logico) {
        case 0:
            if (visitantes_param > 0) {
                printf("[SISTEMA] Se agregaron %d visitantes con éxito a la zona ID %d.\n", cantidad_buf, id_zona_buf);
            } else {
                printf("[SISTEMA] Se removieron %d visitantes con éxito de la zona ID %d.\n", cantidad_buf, id_zona_buf);
            }
            break;

        case -1:
            printf("[ERROR] Lista nula: La estructura global de zonas no ha sido inicializada.\n");
            break;

        case -2:
            printf("[ERROR] No encontrada: La zona con ID %d no existe en el parque.\n", id_zona_buf);
            break;

        case -3:
            printf("[ERROR] Capacidad excedida: El ingreso de %d visitantes supera la capacidad máxima de la zona.\n", cantidad_buf);
            break;

        case -4:
            printf("[ERROR] Supera cantidad mínima: No se pueden remover %d visitantes porque el aforo quedaría menor a cero.\n", cantidad_buf);
            break;
        case -5:
            printf("[ERROR] La Zona ya está cerrada.\n");
            break;
        default:
            printf("[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}


void mostrar_submenu_atracciones() {
    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                             ++ ATRACCIONES ++\n"
        "=========================================================================\n"
        "    Selecciona una opción escribiendo su número y presionando ENTER.\n"
        "                       Para volver atrás escriba '0'\n"
        "=========================================================================\n\n");
    printf(
        "[1] Agregar Atracción a Zona\n"
        "[2] Eliminar Atracción de Zona\n"
        "[3] Mover Atracción a Distinta Zona\n"
        "[4] Modificar Estado de Atracción\n\n"

        "=========================================================================\n\n"
    );
}

void menu_agregar_atraccion_zona(struct NodoZonas *head_zonas) {
    char linea[256];
    char token_control[40];
    char nombre_buf[40];
    char tematica_buf[30];
    char basura;

    int id_zona_buf;
    int duracion_buf;
    int cap_max_buf;
    int edad_min_buf;
    float altura_min_buf;

    int asignados;
    int es_valido;
    int resultado_logico;
    struct Zona *zona_destino;

    es_valido = 0;
    zona_destino = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                     ++ AGREGAR ATRACCIÓN A ZONA ++\n"
        "=========================================================================\n"
        "       Para agregar una atracción, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID Zona, Nombre, Temática, Duración (minutos), \n"
        "    Capacidad por Ciclo, Edad Mínima, y Altura Mínima (metros).\n"
        "[2] Textos como el Nombre no deben contener espacios (usar '_')."
        "Por ejemplo: '3 Trueno_del_Dragon Aventura 4 24 10 1.20'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%39s", token_control) == 1) {
            if (strcmp(token_control, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %39s %29s %d %d %d %f %c",
                           &id_zona_buf, nombre_buf, tematica_buf, &duracion_buf,
                           &cap_max_buf, &edad_min_buf, &altura_min_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningun ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 7) {
            printf("[ERROR] Datos incompletos. Se procesaron solo %d de los 7 campos requeridos.\n\n", asignados);
            continue;
        }

        if (asignados > 7) {
            printf("[ERROR] Entrada inválida. Detectados elementos desconocidos al final de la línea.\n\n");
            continue;
        }

        if (id_zona_buf <= 0 || duracion_buf <= 0 || cap_max_buf <= 0 || edad_min_buf < 0 || altura_min_buf < 0.0f) {
            printf("[ERROR] Valores numericos incoherentes. Verifique que las cantidades sean positivas.\n\n");
            continue;
        }

        zona_destino = obtener_zona_por_id(head_zonas, id_zona_buf);
        if (zona_destino == NULL) {
            printf("[ERROR] La zona con ID %d no existe en el sistema del parque.\n\n", id_zona_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = agregar_atraccion(head_zonas, zona_destino, nombre_buf, tematica_buf,
                                         duracion_buf, cap_max_buf, edad_min_buf, altura_min_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] ¡Atraccion '%s' agregada con exito a la zona ID %d!\n", nombre_buf, id_zona_buf);
            break;

        case -2:
            printf("[ERROR] Incompatibilidad: La tematica '%s' no coincide con la identidad de la zona.\n", tematica_buf);
            break;

        case -3:
            printf("[ERROR] Capacidad excedida: La zona ID %d no tiene espacio fisico para mas atracciones.\n", id_zona_buf);
            break;

        default:
            printf("[ERROR] Operacion rechazada. Codigo de error no especificado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menú principal...");
    while (getchar() != '\n');
}

void menu_eliminar_atraccion_zona(struct NodoZonas *head_zonas) {
    char linea[100];
    char token[30];
    char basura;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_atraccion_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                    ++ ELIMINAR ATRACCIÓN DE ZONA ++\n"
        "=========================================================================\n"
        "     Para eliminar una atracción, rellene los siguientes parámetros \n"
        "                         en el orden que se indica.\n"
        "                Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] El único parámetro es la ID de la atracción.\n"
        "Por ejemplo: '3'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %c", &id_atraccion_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. Debe ingresar un ID numérico entero.\n\n");
            continue;
        }

        if (asignados > 1) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_atraccion_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = eliminar_atraccion(head_zonas, id_atraccion_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] Atracción con ID %d eliminada exitosamente del parque.\n", id_atraccion_buf);
            break;

        case -1:
            printf("[ERROR] Error de sistema: La estructura de Zonas es nula.\n");
            break;

        case -2:
            printf("[ERROR] No se pudo eliminar: No se encontró ninguna atracción con el ID %d.\n", id_atraccion_buf);
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error desconocido (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');
}

void menu_mover_atraccion_distinta_zona(struct NodoZonas *head_zonas) {
    char linea[150];
    char token[30];
    char basura;
    int id_objetivo_buf;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;
    struct Zona *zona_objetivo;

    es_valido = 0;
    id_objetivo_buf = 0;
    id_atraccion_buf = 0;
    zona_objetivo = NULL;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                      ++ MOVER ATRACCION A ZONA ++\n"
        "=========================================================================\n"
        "       Para mover una atracción, rellene los siguientes parámetros \n"
        "                        en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID Zona objetivo, ID Atracción.\n"
        "[2] Deben estar separados por espacios.\n"
        "Por ejemplo: '3 23'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %d %c", &id_objetivo_buf, &id_atraccion_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] No se detectó ningún ingreso válido. Intente de nuevo.\n\n");
            continue;
        }

        if (asignados < 2) {
            printf("[ERROR] Datos incompletos. Debe ingresar la ID de la zona objetivo y la ID de la atracción.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_objetivo_buf <= 0 || id_atraccion_buf <= 0) {
            printf("[ERROR] IDs inválidas. Los identificadores deben ser números enteros positivos.\n\n");
            continue;
        }

        zona_objetivo = obtener_zona_por_id(head_zonas, id_objetivo_buf);
        if (zona_objetivo == NULL) {
            printf("[ERROR] La zona objetivo con ID %d no existe en el sistema del parque.\n\n", id_objetivo_buf);
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = mover_atraccion(head_zonas, zona_objetivo, id_atraccion_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] ¡Atracción con ID %d movida con éxito a la zona objetivo %d!\n",
                   id_atraccion_buf, id_objetivo_buf);
            break;

        case -2:
            printf("[ERROR] Incompatibilidad: La temática de la atracción no coincide con la identidad de la zona objetivo.\n");
            break;

        case -3:
            printf("[ERROR] Capacidad excedida: La zona objetivo con ID %d llegó al máximo de su capacidad.\n", id_objetivo_buf);
            break;

        case -4:
            printf("[ERROR] No encontrada: No se encontró ninguna atracción con el ID %d en ninguna zona del parque.\n", id_atraccion_buf);
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');

}

void menu_modificar_estado_atraccion(struct NodoZonas *head_zonas) {
    char linea[150];
    char token[30];
    char estado_buf[30];
    char basura;
    int id_atraccion_buf;
    int asignados;
    int es_valido;
    int resultado_logico;

    es_valido = 0;
    id_atraccion_buf = 0;

    limpiar_pantalla();

    printf(
        "=========================================================================\n"
        "                   ++ MODIFICAR ESTADO DE ATRACCION ++\n"
        "=========================================================================\n"
        "     Para modificar una atracción, rellene los siguientes parámetros \n"
        "                        en el orden que se indica.\n"
        "                 Para volver atrás y cancelar escriba 'volver'\n"
        "=========================================================================\n\n");
    printf(
        "- INFORMACIÓN\n"
        "[1] Los parámetros son: ID de Atracción, y Nuevo Estado.\n"
        "[2] Los estados posibles son: 'operativa', 'mantenimiento', 'cerrada' y,\n"
        "'fuera_de_servicio'.\n"
        "[3] Todo debe estar en minúsculas y sin espacios.\n"
        "Por ejemplo: '42 mantenimiento'\n\n"

        "=========================================================================\n\n"
    );

    while (!es_valido) {
        printf(">> ");

        if (fgets(linea, (int)sizeof(linea), stdin) == NULL) {
            printf("[ERROR] Error crítico al leer desde el teclado.\n\n");
            continue;
        }

        if (sscanf(linea, "%29s", token) == 1) {
            if (strcmp(token, "volver") == 0) {
                return;
            }
        }

        asignados = sscanf(linea, "%d %29s %c", &id_atraccion_buf, estado_buf, &basura);

        if (asignados <= 0) {
            printf("[ERROR] Formato incorrecto. El primer parámetro debe ser el ID numérico de la atracción.\n\n");
            continue;
        }

        if (asignados == 1) {
            printf("[ERROR] Datos incompletos. Debe ingresar el nuevo estado después del ID.\n\n");
            continue;
        }

        if (asignados > 2) {
            printf("[ERROR] Entrada inválida. Escribió argumentos de más al final de la línea.\n\n");
            continue;
        }

        if (id_atraccion_buf <= 0) {
            printf("[ERROR] ID inválido. El identificador debe ser un número entero positivo.\n\n");
            continue;
        }

        if (strcmp(estado_buf, "operativa") != 0 &&
            strcmp(estado_buf, "mantenimiento") != 0 &&
            strcmp(estado_buf, "cerrada") != 0 &&
            strcmp(estado_buf, "fuera_de_servicio") != 0) {

            printf("[ERROR] El estado '%s' no es válido.\n", estado_buf);
            printf("[INFO] Estados permitidos: [operativa, mantenimiento, cerrada, fuera_de_servicio]\n\n");
            continue;
        }

        es_valido = 1;
    }

    resultado_logico = cambiar_estado_atraccion(head_zonas, id_atraccion_buf, estado_buf);

    switch (resultado_logico) {
        case 0:
            printf("[SISTEMA] Estado de la atracción %d actualizado a '%s' con éxito\n",
                   id_atraccion_buf, estado_buf);
            break;

        case -1:
            printf("[ERROR] Error de sistema: La estructura de Zonas es nula (NULL).\n");
            break;

        case -2:
            printf("[ERROR] No encontrada: No se encontró ninguna atracción con el ID %d en el parque.\n", id_atraccion_buf);
            break;

        case -3:
            printf("[ERROR] Error crítico: Falta memoria en el sistema para procesar el cambio.\n");
            break;

        default:
            printf("[ALERTA] Operación rechazada. Código de error no catalogado (%d).\n", resultado_logico);
            break;
    }

    printf("\nPresione ENTER para regresar al menu principal...");
    while (getchar() != '\n');

}

/*
====================================
---------------MAIN-----------------
====================================
*/

int main(void) {
    struct Parque *parque = NULL;
    int opcion_principal;
    int continuar_programa;

    continuar_programa = 1;

    parque = (struct Parque *)malloc(sizeof(*parque));
    if (!menu_inicializar_parque(parque)) return -1;

    while (continuar_programa) {
        limpiar_pantalla();
        mostrar_menu_principal();
        opcion_principal = escoger_opcion();

        switch (opcion_principal) {
            case 1:
                ejecutar_submenu_entradas(&(parque->head_entradas));
                continue;
            case 2:
                ejecutar_submenu_visitantes(parque);
                continue;
            case 3:
                ejecutar_submenu_filas(parque);
                continue;
            case 4:
                ejecutar_submenu_zonas(&(parque->head_zonas));
                continue;
            case 5:
                ejecutar_submenu_atracciones(parque->head_zonas);
                continue;
            case 6:
                ver_reporte_general_dia(parque);
                continue;
            case 7:
                ver_reporte_atracciones_actual(parque->head_zonas);
                continue;
            case 8:
                ver_atracciones_actuales(parque->head_zonas);
                continue;
            case 9:
                ver_zonas_actuales(parque->head_zonas);
                continue;
            case 10:
                ver_visitantes_parque(parque->raiz_visitantes);
                continue;
            case 11:
                ver_entradas_compradas(parque->head_entradas);
                continue;
            case 12:
                ver_filas_atracciones(parque->head_zonas);
                continue;
            case 0:
                continuar_programa = 0;
                break;
            default:
                printf("\n[AVISO] Opcion inválida. Ingrese un numero entre 0 y 12.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }

    return 0;
}

void ejecutar_submenu_entradas(struct NodoEntradas **entradas) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        limpiar_pantalla();
        mostrar_submenu_entradas();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_comprar_entrada(entradas);
                return;
            case 2:
                menu_eliminar_entrada(entradas);
                return;
            case 3:
                menu_cambiar_estado_entrada(entradas);
                return;
            case 0:
                continuar = 0;
                return;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_visitantes(struct Parque *parque) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        limpiar_pantalla();
        mostrar_submenu_visitantes();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_visitante(parque);
                return;
            case 2:
                menu_eliminar_visitante(&(parque->raiz_visitantes));
                return;
            case 0:
                continuar = 0;
                break;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_filas(struct Parque *parque) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        limpiar_pantalla();
        mostrar_submenu_filas();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_grupo_fila(parque);
                return;
            case 2:
                menu_avanzar_fila_atraccion(parque->head_zonas);
                return;
            case 0:
                continuar = 0;
                return;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_zonas(struct NodoZonas **head_zonas) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        limpiar_pantalla();
        mostrar_submenu_zonas();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_zona_al_parque(head_zonas);
                return;
            case 2:
                menu_eliminar_zona_del_parque(head_zonas);
                return;
            case 3:
                menu_agregar_o_remover_visitante_zona(*head_zonas);
                return;
            case 0:
                continuar = 0;
                break;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}

void ejecutar_submenu_atracciones(struct NodoZonas *head_zonas) {
    int sub_opcion;
    int continuar;
    continuar = 1;

    while (continuar) {
        limpiar_pantalla();
        mostrar_submenu_atracciones();
        sub_opcion = escoger_opcion();

        switch (sub_opcion) {
            case 1:
                menu_agregar_atraccion_zona(head_zonas);
                return;
            case 2:
                menu_eliminar_atraccion_zona(head_zonas);
                return;
            case 3:
                menu_mover_atraccion_distinta_zona(head_zonas);
                return;
            case 4:
                menu_modificar_estado_atraccion(head_zonas);
                return;
            case 0:
                continuar = 0;
                return;
            default:
                printf("\n[AVISO] Opcion inválida.\n");
                printf("Presione ENTER para continuar...");
                while (getchar() != '\n');
        }
    }
}