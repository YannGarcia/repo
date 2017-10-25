(setq-default indent-tabs-mode nil)
(setq-default tab-width 2)
(setq-default show-trailing-whitespace t)
;;
(show-paren-mode t)
(setq show-paren-delay 0)
;;
(require 'ttcn3)
(setq auto-mode-alist (cons '("\\.ttcn3?" . ttcn-3-mode) auto-mode-alist))
;;
(global-set-key "%" 'match-paren)
;; VIM % functionality
(defun match-paren (arg)
  "Go to the matching paren if on a paren; otherwise insert %."
  (interactive "p")
  (cond ((looking-at "\\s(") (forward-list 1) (backward-char 1))
	((looking-at "\\s)") (forward-char 1) (backward-list 1))
	(t (self-insert-command (or arg 1)))))
;;
(add-to-list 'auto-mode-alist '("\\.partH\\'" . c++-mode))
(add-to-list 'auto-mode-alist '("\\.partC\\'" . c++-mode))
(put 'upcase-region 'disabled nil)
