-- ============================================================================
-- Création du schéma des historiques de personnage
-- ============================================================================

-- TABLE: HISTORIQUE
CREATE TABLE IF NOT EXISTS historiques (
    id                     UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    nom                    VARCHAR(100) NOT NULL UNIQUE,    -- Nom de l'historique
    alias                  VARCHAR(100) NOT NULL UNIQUE,    -- Nom alternatif pour pseudo markdown
    description            TEXT,                            -- Description de l'historique
    competences            TEXT[],                          -- Liste de compétences (noms)
    equipement_id          UUID[],                          -- liste séparée par des virgule de réf (vers equipements.id)
);

-- TABLE: HISTORIQUE_TRAIT
CREATE TABLE IF NOT EXISTS historique_traits (
    id                      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    historique_id           UUID REFERENCES historiques(id) ON DELETE CASCADE, -- Référence à l'historique
    nom                     VARCHAR(100) NOT NULL,           -- Nom du trait de l'historique
    description             TEXT NOT NULL                               -- Description du trait de l'historique
);

-- TABLE: HISTORIQUE_IDEAL
CREATE TABLE IF NOT EXISTS historique_ideaux (
    id                      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    historique_id           UUID REFERENCES historiques(id) ON DELETE CASCADE, -- Référence à l'historique
    nom                     VARCHAR(100) NOT NULL,           -- Nom du trait de l'historique
    description             TEXT NOT NULL                               -- Description du trait de l'historique
);

-- TABLE: HISTORIQUE_LIEN
CREATE TABLE IF NOT EXISTS historique_liens (
    id                      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    historique_id           UUID REFERENCES historiques(id) ON DELETE CASCADE, -- Référence à l'historique
    nom                     VARCHAR(100) NOT NULL,           -- Nom du trait de l'historique
    description             TEXT NOT NULL                               -- Description du trait de l'historique
);

-- TABLE: HISTORIQUE_DEFAUT
CREATE TABLE IF NOT EXISTS historique_defauts (
    id                      UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    historique_id           UUID REFERENCES historiques(id) ON DELETE CASCADE, -- Référence à l'historique
    nom                     VARCHAR(100) NOT NULL,           -- Nom du trait de l'historique
    description             TEXT NOT NULL                               -- Description du trait de l'historique
);
