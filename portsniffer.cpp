#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <pthread.h>

// Lista de subdomínios comuns
const char *subdominios[] = {
    "www", "mail", "ftp", "ns", "webmail", "smtp", "pop", "imap", "vpn", "blog"
};
const int num_subdominios = 10;

// Lista de portas comuns e seus serviços associados
struct {
    int porta;
    const char *servico;
} portas_comuns[] = {
    {21, "FTP"},
    {22, "SSH"},
    {23, "Telnet"},
    {25, "SMTP"},
    {53, "DNS"},
    {80, "HTTP"},
    {110, "POP3"},
    {143, "IMAP"},
    {443, "HTTPS"},
    {3306, "MySQL"},
    {3389, "RDP"},
    {8080, "HTTP Alt"},
    {8443, "HTTPS Alt"}
};
const int num_portas_comuns = 13;

// Estrutura para passar dados para as threads
typedef struct {
    char ip[100];
    int porta;
    const char *servico;
} ThreadData;

// Função para obter e exibir os endereços associados a um host
void obterEnderecos(char *host) {
    struct addrinfo hints, *res, *p;
    char ipstr[INET6_ADDRSTRLEN];
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Pode ser IPv4 ou IPv6
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return;
    }

    printf("Endereços IP de %s:\n", host);

    for(p = res; p != NULL; p = p->ai_next) {
        void *addr;

        // Obter o endereço e a família do socket
        if (p->ai_family == AF_INET) {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
        } else {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
        }

        // Converter o IP para uma string
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s\n", ipstr);
    }

    freeaddrinfo(res);
}

// Função para obter IP de subdomínios
void obterSubdominios(char *dominio) {
    char subdominio[100];
    for (int i = 0; i < num_subdominios; i++) {
        snprintf(subdominio, sizeof subdominio, "%s.%s", subdominios[i], dominio);
        printf("\nProcurando subdomínio: %s\n", subdominio);
        obterEnderecos(subdominio);
    }
}

// Função para verificar se a porta está aberta (usada pelas threads)
void *verificarPortaThread(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int sockfd;
    struct sockaddr_in target;
    struct timeval timeout;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Erro ao criar socket");
        return NULL;
    }

    // Definir tempo de espera para conexões
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof timeout);

    target.sin_family = AF_INET;
    target.sin_port = htons(data->porta);
    inet_pton(AF_INET, data->ip, &target.sin_addr);

    // Tentar conectar ao alvo
    if (connect(sockfd, (struct sockaddr *)&target, sizeof(target)) < 0) {
        printf("%d\t\t%s\t\tFECHADA\n", data->porta, data->servico);
    } else {
        printf("%d\t\t%s\t\tABERTA\n", data->porta, data->servico);
    }

    close(sockfd);
    free(data); // Liberar memória alocada para a estrutura
    return NULL;
}

// Função para realizar a varredura nas portas comuns usando threads
void varreduraPortasComuns(char *host, char *ip) {
    printf("Iniciando varredura de portas comuns no host %s (IP: %s)...\n", host, ip);
    printf("Porta\t\tServiço\t\tStatus\n");
    printf("----------------------------------------\n");

    pthread_t threads[num_portas_comuns];
    for (int i = 0; i < num_portas_comuns; i++) {
        // Alocar memória para os dados da thread
        ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
        strcpy(data->ip, ip);
        data->porta = portas_comuns[i].porta;
        data->servico = portas_comuns[i].servico;

        // Criar uma thread para verificar a porta
        if (pthread_create(&threads[i], NULL, verificarPortaThread, (void *)data) != 0) {
            perror("Erro ao criar thread");
            free(data); // Liberar memória em caso de erro
        }
    }

    // Aguardar todas as threads terminarem
    for (int i = 0; i < num_portas_comuns; i++) {
        pthread_join(threads[i], NULL);
    }
}

// Função principal usando argc e argv
int main(int argc, char *argv[]) {
    char host[100];
    char ip[100];

    // Se argumentos foram passados pela linha de comandos
    if (argc == 2) {
        strcpy(host, argv[1]);
        obterSubdominios(host);
        printf("\nDigite o endereço IP para varredura de portas: ");
        scanf("%s", ip);
        varreduraPortasComuns(host, ip);
    } else {
        // Modo interativo
        printf("Digite o host (nome de domínio ou IP): ");
        scanf("%s", host);
        obterSubdominios(host);
        printf("\nDigite o endereço IP para varredura de portas: ");
        scanf("%s", ip);
        varreduraPortasComuns(host, ip);
    }

    return 0;
}
