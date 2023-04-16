#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	if (config_has_property(config, "IP"))
	{
		ip = config_get_string_value(config, "IP");
	}

	if (config_has_property(config, "PUERTO"))
	{
		puerto = config_get_string_value(config, "PUERTO");
	}

	if (config_has_property(config, "CLAVE"))
	{
		valor = config_get_string_value(config, "CLAVE");
	}
	// Loggeamos el valor de config
    log_info(logger, ip);
    log_info(logger, puerto);
    log_info(logger, valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log *nuevo_logger;

    nuevo_logger = log_create("./logs/tp0.log", "tp0", true, LOG_LEVEL_INFO);

    if (nuevo_logger == NULL)
    {
        printf("No se pudo iniciar el logger!");
        exit(2);
    }

    return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config *nuevo_config;

	nuevo_config = config_create("./cfg/cliente.config");

	if (nuevo_config == NULL)
	{
		printf("No se pudo iniciar el config!");
		exit(2);
	}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
    char *leido;

    // La primera te la dejo de yapa
    leido = readline("> ");

    // El resto, las vamos leyendo y logueando hasta recibir un string vacío
    while (strcmp(leido, "") != 0)
    {
        log_info(logger, leido);
        free(leido);
        leido = NULL;
        leido = readline("> ");
    }

    // ¡No te olvides de liberar las lineas antes de regresar!
    free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete *paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	int tamanio = strlen(leido) + 1;

	while (strcmp(leido, "") != 0)
	{
		agregar_a_paquete(paquete, leido, tamanio);
		free(leido);
		leido = NULL;
		leido = readline("> ");
		tamanio = strlen(leido) + 1;
	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	enviar_paquete(paquete, conexion);
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if (logger != NULL)
	{
		log_destroy(logger);
		printf("Logger destruido exitosamente");
	}

	if (config != NULL)
	{
		config_destroy(config);
		printf("Config destruida exitosamente");
	}

	if (conexion != 0)
	{
		liberar_conexion(conexion);
		printf("Conexión liberada exitosamente");
	}
}
