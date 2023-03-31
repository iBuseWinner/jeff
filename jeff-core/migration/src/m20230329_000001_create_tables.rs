use sea_orm_migration::prelude::*;

#[derive(DeriveMigrationName)]
pub struct Migration;

#[async_trait::async_trait]
impl MigrationTrait for Migration {
  async fn up(&self, manager: &SchemaManager) -> Result<(), DbErr> {
    manager
      .create_table(
        Table::create()
          .table(Users::Table)
          .if_not_exists()
          .col(ColumnDef::new(Users::Id).big_integer().not_null().auto_increment().primary_key().unique_key())
          .col(ColumnDef::new(Users::Login).string().not_null().unique_key())
          .col(ColumnDef::new(Users::PHash).string().not_null())
          .col(ColumnDef::new(Users::Settings).string().not_null())
          .to_owned()
      ).await?;
    manager
      .create_table(
        Table::create()
          .table(Phrases::Table)
          .if_not_exists()
          .col(ColumnDef::new(Phrases::Id).big_integer().not_null().auto_increment().primary_key().unique_key())
          .col(ColumnDef::new(Phrases::UserId).big_integer().not_null())
          .col(ColumnDef::new(Phrases::Phrase).string().not_null())
          .col(ColumnDef::new(Phrases::Links).string())
          .col(ColumnDef::new(Phrases::Exec).boolean().not_null())
          .col(ColumnDef::new(Phrases::Properties).string().not_null())
          .to_owned()
      ).await
  }

  async fn down(&self, manager: &SchemaManager) -> Result<(), DbErr> {
    manager.drop_table(Table::drop().table(Users::Table).to_owned()).await?;
    manager.drop_table(Table::drop().table(Phrases::Table).to_owned()).await
  }
}

/// Learn more at https://docs.rs/sea-query#iden
#[derive(Iden)]
enum Users {
  Table,
  Id,
  Login,
  PHash,
  Settings,
}

#[derive(Iden)]
enum Phrases {
  Table,
  Id,
  UserId,
  Phrase,
  Links,
  Exec,
  Properties,
}

