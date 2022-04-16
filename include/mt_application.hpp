#ifndef B8D83158_3027_49A9_BAA8_0A7468DAD697
#define B8D83158_3027_49A9_BAA8_0A7468DAD697

#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <cstdlib>
#include <sstream>
#include <mutex>

#include "mt_renderer.hpp"

#define BLACK_UP_POINTING_TRIANGLE "\u25B2"
#define BLACK_DOWN_POINTING_TRIANGLE "\u25BC"
#define BLACK_LEFT_POINTING_TRIANGLE "\u25C0"
#define BLACK_RIGHT_POINTING_TRIANGLE "\u25B6"

namespace TOOLKIT_NAMESPACE
{
	class Window;
	class Application
	{
	private:
		bool running = false;

		std::map<std::pair<std::string, int>, TTF_Font *> fonts;
		std::vector<std::thread *> coroutines;

		Uint64 fStart = 0;
		Uint64 frameTime = 0;

		double frameDelay;

		const unsigned short targetFPS = 60;

	public:
		/**
		 * @brief Constrói uma aplicação dados o título, e as dimensões da janela.
		 *
		 * @param title Título da janela.
		 * @param width Largura da janela.
		 * @param height Altura da janela.
		 * @param flags Atributos adicionais. São válidos: FULLSCREEN, SHOWN, HIDDEN, BORDERLESS, RESIZABLE, MINIMIZED, MAXIMIZED, FULLSCREEN_DESKTOP, ALWAYS_ON_TOP, SKIP_TASKBAR, UTILITY, TOOLTIP, POPUP_MENU. Use o operador | para especificar mais de um atributo.
		 */
		Application(const std::string &title, int width, int height, int flags = 0);
		~Application();

		/**
		 * @brief Estrutura de eventos do aplicativo (somente leitura).
		 *
		 */
		SDL_Event event;

		/**
		 * @brief Ponteiro para a janela principal do aplicativo.
		 *
		 */
		Window *window = nullptr;

		/**
		 * @brief Permite a execução de procedimentos e funções em paralelo à execução do aplicativo.
		 *
		 * @tparam T Tipo da função. Pode ser expressa como uma std::function, por exemplo: std::function<void(int,int)>. Neste caso temos um procedimento do tipo void que recebe dois tipos inteiros. Pode ser omitido caso o tipo da função possa ser deduzido e seus parâmetros não precisem ser explicitados.
		 * @tparam tArgs Tipos dos argumentos da função. Os tipos podem ser omitidos nos casos em que possam ser deduzidos. Caso algum dos parâmetros da função sejam passados por referência, por exemplo, os argumentos devem ser informados e as referências precisam ser passadas na forma std::ref(Arg).
		 * @param function Função ou procedimento a ser executado paralelamente. Caso o o aplicativo receba um evento de parada, será aguardado seu fim para então ser encerrado.
		 * @param args Valores a serem passados para a função.
		 */
		template <typename T, typename... tArgs>
		void newCoroutine(T function, tArgs... args)
		{
			auto thread = new std::thread(function, args...);
			coroutines.emplace_back(thread);
			thread->detach();
		}

		/**
		 * @brief Retorna uma fonte com o tamanho especificado.
		 *
		 * @param path Caminho para o arquivo da fonte.
		 * @param fontSize Tamanho da fonte.
		 * @return Um ponteiro do tipo TTF_Font caso a fonte seja carregada com sucesso. Retorna nullptr caso contrário.
		 */
		TTF_Font *getFont(const std::string &path, int fontSize);

		/**
		 * @brief Verifica o estado de execução do aplicativo
		 *
		 * @return Retorna true caso esteja executando, retorna false caso contrário, ou se recebeu um sinal de parada.
		 */
		bool isRunning() const noexcept
		{
			return running;
		}

		/**
		 * @brief Operador de execução.
		 *
		 * @return Retorna um inteiro referente ao sucesso da execução.
		 */
		int operator()();
		/**
		 * @brief Executa a aplicação. Equivalente ao operador de parênteses '( )'
		 * @return Retorna um inteiro referente ao sucesso da execução.
		 */
		int run();
		/**
		 * @brief Envia um sinal de parada para o aplicativo.
		 *
		 */
		void stop();
	};
}

#endif /* B8D83158_3027_49A9_BAA8_0A7468DAD697 */
