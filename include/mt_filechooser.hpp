#ifndef F1F7046B_7A85_48B5_97B4_203A7C1170A1
#define F1F7046B_7A85_48B5_97B4_203A7C1170A1

#include "mt_core.hpp"

#if defined(_WIN32)

#include <windows.h>

#define ALL_FILES                      \
    {                                  \
        "Todos os arquivos", { "*.*" } \
    }

namespace TOOLKIT_NAMESPACE
{
    namespace fileChooserFlags
    {
        enum fileChooserFlags
        {
            NONE,
            /**
             * @brief Faz com que a caixa de seleção Somente leitura seja selecionada inicialmente quando a caixa de diálogo é criada. Esse sinalizador indica o estado da caixa de seleção Somente leitura quando a caixa de diálogo é fechada.
             *
             */
            READONLY = OFN_READONLY,
            /**
             * @brief Faz com que a caixa de diálogo Salvar como gere uma caixa de mensagem se o arquivo selecionado já existir. O usuário deve confirmar se deseja substituir o arquivo.
             *
             */
            OVERWRITE_PROMPT = OFN_OVERWRITEPROMPT,
            /**
             * @brief Oculta a caixa de seleção Somente leitura.
             *
             */
            HIDE_READONLY = OFN_HIDEREADONLY,
            /**
             * @brief Restaura o diretório atual para seu valor original se o usuário alterou o diretório enquanto procurava por arquivos. Esse sinalizador é ineficaz para GetOpenFileName.
             *
             */
            NO_CHANGE_DIR = OFN_NOCHANGEDIR,
            /**
             * @brief As caixas de diálogo comuns permitem caracteres inválidos no nome do arquivo retornado.
             *
             */
            NO_VALIDATE = OFN_NOVALIDATE,
            /**
             * @brief A caixa de listagem Nome do arquivo permite várias seleções. Se você também definir o sinalizador OFN_EXPLORER, a caixa de diálogo usará a interface do usuário no estilo Explorer; caso contrário, ele usa a interface de usuário de estilo antigo.
             *
             */
            ALLOW_MULTISELECT = OFN_ALLOWMULTISELECT,
            /**
             * @brief O usuário pode digitar apenas caminhos e nomes de arquivo válidos. Se este sinalizador for usado e o usuário digitar um caminho e um nome de arquivo inválidos no campo de entrada Nome do arquivo, a função da caixa de diálogo exibirá um aviso em uma caixa de mensagem.
             *
             */
            PATH_MUST_EXIST = OFN_PATHMUSTEXIST,
            /**
             * @brief O usuário pode digitar apenas nomes de arquivos existentes no campo de entrada Nome do Arquivo. Se este sinalizador for especificado e o usuário inserir um nome inválido, o procedimento da caixa de diálogo exibirá um aviso em uma caixa de mensagem. Se esse sinalizador for especificado, o sinalizador OFN_PATHMUSTEXIST também será usado. Este sinalizador pode ser usado em uma caixa de diálogo Abrir. Ele não pode ser usado com uma caixa de diálogo Salvar como.
             *
             */
            FILE_MUST_EXIST = OFN_FILEMUSTEXIST,
            /**
             * @brief Se o usuário especificar um arquivo que não existe, esse sinalizador fará com que a caixa de diálogo solicite permissão ao usuário para criar o arquivo. Se o usuário optar por criar o arquivo, a caixa de diálogo será fechada e a função retornará o nome especificado; caso contrário, a caixa de diálogo permanece aberta. Se você usar esse sinalizador com o sinalizador OFN_ALLOWMULTISELECT, a caixa de diálogo permitirá que o usuário especifique apenas um arquivo inexistente.
             *
             */
            CREATE_PROMPT = OFN_CREATEPROMPT,
            /**
             * @brief O arquivo retornado não tem a caixa de seleção Somente leitura selecionada e não está em um diretório protegido contra gravação.
             *
             */
            NO_READONLY_RETURN = OFN_NOREADONLYRETURN,
            /**
             * @brief O arquivo não é criado antes que a caixa de diálogo seja fechada. Esse sinalizador deve ser especificado se o aplicativo salvar o arquivo em um compartilhamento de rede de criação e não modificação. Quando um aplicativo especifica esse sinalizador, a biblioteca não verifica a proteção contra gravação, um disco cheio, uma porta de unidade aberta ou proteção de rede. Os aplicativos que usam esse sinalizador devem executar as operações de arquivo com cuidado, pois um arquivo não pode ser reaberto depois de fechado.
             *
             */
            NO_TEST_FILE_CREATE = OFN_NOTESTFILECREATE,
            /**
             * @brief Oculta e desativa o botão Rede.
             *
             */
            NO_NETWORK_BUTTON = OFN_NONETWORKBUTTON,
            /**
             * @brief Indica que quaisquer personalizações feitas na caixa de diálogo Abrir ou Salvar como usam os métodos de personalização no estilo do Explorer.
             *
             */
            EXPLORER = OFN_EXPLORER,
            /**
             * @brief Direciona a caixa de diálogo para retornar o caminho e o nome do arquivo de atalho selecionado (.LNK). Se esse valor não for especificado, a caixa de diálogo retornará o caminho e o nome do arquivo referenciado pelo atalho.
             *
             */
            NO_DEREFERENCE_LINKS = OFN_NODEREFERENCELINKS,
            /**
             * @brief Permite que a caixa de diálogo estilo Explorer seja redimensionada usando o mouse ou o teclado. Por padrão, as caixas de diálogo Abrir e Salvar como no estilo Explorer permitem que a caixa de diálogo seja redimensionada, independentemente de esse sinalizador estar definido. Esse sinalizador é necessário apenas se você fornecer um procedimento de gancho ou modelo personalizado. A caixa de diálogo de estilo antigo não permite redimensionamento.
             *
             */
            ENABLE_SIZING = OFN_ENABLESIZING,
            /**
             * @brief Impede que o sistema adicione um link ao arquivo selecionado no diretório do sistema de arquivos que contém os documentos usados mais recentemente pelo usuário.
             *
             */
            DONT_ADD_TO_RECENT = OFN_DONTADDTORECENT,
            /**
             * @brief Força a exibição do sistema e arquivos ocultos, substituindo assim a configuração do usuário para mostrar ou não mostrar arquivos ocultos. No entanto, um arquivo marcado como sistema e oculto não é mostrado.
             *
             */
            FORCE_SHOW_HIDDEN = OFN_FORCESHOWHIDDEN,

            // EX_NOPLACESBAR = OFN_EX_NOPLACESBAR,
            // SHARE_FALLTHROUGH = OFN_SHAREFALLTHROUGH,
            // SHARE_NOWARN = OFN_SHARENOWARN,
            // SHARE_WARN = OFN_SHAREWARN,
            // SHOW_HELP = OFN_SHOWHELP,
            // ENABLE_HOOK = OFN_ENABLEHOOK,
            // ENABLE_TEMPLATE = OFN_ENABLETEMPLATE,
            // ENABLE_TEMPLATE_HANDLE = OFN_ENABLETEMPLATEHANDLE,
            // EXTENSION_DIFFERENT = OFN_EXTENSIONDIFFERENT,
            // SHARE_AWARE = OFN_SHAREAWARE,
            // NO_LONG_NAMES = OFN_NOLONGNAMES,
            // LONG_NAMES = OFN_LONGNAMES,
            // ENABLE_INCLUDE_NOTIFY = OFN_ENABLEINCLUDENOTIFY,
        };
    }

    class FileChooser
    {
    private:
        OPENFILENAME chooser;
        char filename[MAX_PATH];
        int errorlevel = 0;

        void catchReturn();

    public:
        FileChooser();

        /**
         * @brief Define o título do diálogo de seleção de arquivo.
         *
         * @param title Título do diálogo;
         */
        void setTitle(const char *title);
        /**
         * @brief Define os sinalizadores que devem ser usados ao abrir o diálogo de seleção de arquivo.
         *
         * @param flags Sinalizadores a serem usados. Podem ser: NONE, READONLY, OVERWRITE_PROMPT, HIDE_READONLY, NO_CHANGE_DIR, NO_VALIDATE, ALLOW_MULTISELECT, PATH_MUST_EXIST, FILE_MUST_EXIST, CREATE_PROMPT, NO_READONLY_RETURN, NO_TEST_FILE_CREATE, NO_NETWORK_BUTTON, EXPLORER, NO_DEREFERENCE_LINKS, ENABLE_SIZING, DONT_ADD_TO_RECENT. Use o operador '|' para especificar mais de um sinalizador.
         */
        void setFlags(fileChooserFlags::fileChooserFlags flags);
        /**
         * @brief Define os filtros de extensão a serem usados durante a seleção do(s) arquivo(s).
         *
         * @param filters Filtros de extensão. Os filtros são definidos como uma lista de inicializadores, logo, têm que ser construídos dentro de chaves '{}'. Cada filtro é uma tupla de uma cadeia de caracteres e outra lista também de caracteres, que dizem respeito ao nome do filtro e suas extensões abrangidas respectivamente. Exemplo: chooser.setFilter({ {"Todos os arquivos", {"*.*"}}, {"Imagens", {"*.jpg", "*.png", "*.tiff"}} });
         */
        void setFilter(std::initializer_list<std::pair<const char *, std::initializer_list<const char *>>> filters);

        int showOpenDialog();
        int showSaveDialog();
    };
}
#endif

#endif /* F1F7046B_7A85_48B5_97B4_203A7C1170A1 */
